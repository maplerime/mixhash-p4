/* clang-format off */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>

#include <inttypes.h>  //strlen
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>     //write

#define true  (1==1)
#define false (1!=1)

#define ERR_LOG \
	do { \
		fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
		__LINE__, __FILE__, errno, strerror(errno)); exit(1); \
	} while(0)


int reg_chnl = 0;
int dbg_print = 1;
void *map_base, *virt_addr;
uint8_t our_fake_address_space[64*1024*2] = {0};
int proc_server(void);

int log_timestamps = 0;
void log_time(char *why) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  printf("%lld.%.9ld : %s", (long long)ts.tv_sec, ts.tv_nsec, why);
}

#define FPGA_TILE_0 0x22000
#define FPGA_TILE_1 0x23000
#define FPGA_TILE_2 0x24000
#define FPGA_TILE_3 0x25000

uint32_t tile_offset[4] = { FPGA_TILE_0,
                            FPGA_TILE_1,
                            FPGA_TILE_2,
                            FPGA_TILE_3 };
int active_tile = 0;
int active_group = 0;

typedef enum {
  FPGA_CMD_WRITE= 1,
  FPGA_CMD_READ = 2,
  FPGA_CMD_RST  = 3,
  FPGA_CMD_TCK  = 4, // update jtag clock (speed)
} fpga_ctrlr_cmd_e;

#define FPGA_STATUS_DONE  (1 << 16)
#define FPGA_STATUS_ERROR (2 << 16)

typedef struct {
  uint32_t r0_ctrl_status; // [15:0] cmd, [31:16] status
  uint32_t r1_group; // 0-8, 9=top
  uint32_t r2_addr;  // [15:0] offset 
  uint32_t r3_data;  // [15:0] data to write/read
  uint32_t r15_tile_reset;  // [15:0] data to write/read
} fpga_jtag_to_mdio_ctrl_t;

uintptr_t fpga_ctrlr_base_address = 0;
fpga_jtag_to_mdio_ctrl_t *fpga_ctrlr;

fpga_jtag_to_mdio_ctrl_t *fpga_ctrlr_set(void) {
  fpga_ctrlr = (fpga_jtag_to_mdio_ctrl_t *)(uintptr_t)(fpga_ctrlr_base_address + tile_offset[ active_tile ]);
  return fpga_ctrlr;
}

void fpga_active_tile_set(int tile) {
  if (tile < 4) {
    active_tile = tile;
  } else {
    active_tile = 0;
  }
//hack, make all tiles = tile 0
//active_tile = 0;

  fpga_ctrlr_set(); // update ptr
}

int fpga_wait_done(int max_tmout) {
  int tmout = max_tmout;
  uint32_t sts;
  uint32_t raw_sts;

  do {
    raw_sts = (volatile uint32_t)fpga_ctrlr->r0_ctrl_status;

    sts = raw_sts & 0xFFFF0000;
  } while ((sts == 0) && (tmout-- > 0));

  //printf("fpga_ctrlr=%p : status=0x%08x\n", fpga_ctrlr, raw_sts);

  if (sts == FPGA_STATUS_DONE) {
    return 0;
  } else if (sts == FPGA_STATUS_ERROR) {
    printf("..error.. ");
    return -1;
  } else if (tmout <= 0) {
    printf("..timeout..[%d] ", max_tmout);
    return -2;
  }
  printf("..ERROR..[sts=%08x, tmout=%d] ", raw_sts, tmout);
  exit(1);
  //return -3;
}

#define MAX_TILE 4
void fpga_ctrlr_reset(void) {
  int tmout = 100000;
  int tile;

  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    printf("fpga_ctrlr reset tile %d..\n", tile);
    fpga_ctrlr->r15_tile_reset = 0x0;
  }
  sleep(1);
  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    printf("fpga_ctrlr un-reset tile ..\n");
    fpga_ctrlr->r15_tile_reset = 0xf;
  }
  sleep(1);
  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    fpga_ctrlr->r0_ctrl_status = FPGA_CMD_RST;
  }
  sleep(1);
  printf("Set MDIO speed to 2Mhz (divider=30)\n");
  for (tile = 0; tile < MAX_TILE; tile++) {
    fpga_active_tile_set(tile);
    fpga_ctrlr->r3_data = 30; // 2Mhz (62.5Mhz/(div+2)
    fpga_ctrlr->r0_ctrl_status = FPGA_CMD_TCK;
  }
  sleep(1);
}


void fpga_ctrlr_rd(uint32_t addr, uint32_t *data) {
  if (fpga_wait_done(1000) != 0) {
    printf("Previous operation not done (before read)\n");
    *data = 0x00000bad;
    //fpga_ctrlr_init(fpga_ctrlr_base_address);
  }
  if (log_timestamps) log_time("Read start: \n");
  fpga_ctrlr->r1_group = active_group;
  fpga_ctrlr->r2_addr  = addr;
  fpga_ctrlr->r0_ctrl_status = FPGA_CMD_READ;

  if (fpga_wait_done(1000) != 0) {
    printf("Read failed\n");
    *data = 0x00001bad;
    return;
  }
  *data = fpga_ctrlr->r3_data;
  if (log_timestamps) log_time("Read done :\n");
}

void fpga_ctrlr_wr(uint32_t addr, uint32_t data) {
  if (fpga_wait_done(1000) != 0) {
    printf("Previous operation not done (before write)\n");
    //fpga_ctrlr_init(fpga_ctrlr_base_address);
  }
  if (log_timestamps) log_time("Write start:\n");
  fpga_ctrlr->r1_group = active_group;
  fpga_ctrlr->r2_addr  = addr;
  fpga_ctrlr->r3_data  = data;
  fpga_ctrlr->r0_ctrl_status = FPGA_CMD_WRITE;

  if (fpga_wait_done(1000) != 0) {
    printf("Write failed\n");
    return;
  }
  if (log_timestamps) log_time("Write done :\n");
}

/*********************************************************************
* create_server
*********************************************************************/
int create_server(int listen_port, bool local_only) {
  int socket_desc, client_sock, c;
  struct sockaddr_in server, client;
  int so_reuseaddr = 1;

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    printf("ERROR: fpga_intf could not create socket");
    return -1;
  }
  printf("fpga_intf: listen socket created\n");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = local_only ? htonl(INADDR_LOOPBACK) : htonl(INADDR_ANY);
  server.sin_port = htons(listen_port);

  setsockopt(socket_desc,
             SOL_SOCKET,
             SO_REUSEADDR,
             &so_reuseaddr,
             sizeof so_reuseaddr);

  // Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind failed. Error");
    close(socket_desc);
    return -1;
  }
  printf("fpga_intf: bind done on port %d, listening...\n", listen_port);
  do {
    // Listen
    puts("fpga_intf: listening for incoming connections...");
    listen(socket_desc, 1);


    // accept connection from an incoming client
    c = sizeof(struct sockaddr_in);
    client_sock =
        accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    if (client_sock < 0) {
      printf("accept failed");
      //close(socket_desc);
      continue;
    }
    reg_chnl = client_sock;

    printf("fpga_intf: <CONNECT> connection accepted on client sock: %d\n", client_sock);
    proc_server(); // start processing
    printf("fpga_intf: <DISCONNECT> connection terminated on client sock: %d\n", client_sock);

    close(client_sock);
  } while (true);

  return client_sock;
}

/*********************************************************************
* write_to_socket
*********************************************************************/
int write_to_socket(int sock, uint8_t *buf, int len) {
  if (send(sock, buf, len, 0) < 0) {
    printf("write_to_socket failed");
    return -3;
  }
  return 0;
}

/*********************************************************************
* read_from_socket
*********************************************************************/
int read_from_socket(int sock, uint8_t *buf, int len) {
  int n_read = 0, n_read_this_time = 0, i = 1;

  // Receive a message from client
  do {
    n_read_this_time =
        recv(sock, (buf + n_read), (len - n_read), MSG_WAITALL);
        //recv(sock, (buf + n_read), (len - n_read), 0 /*MSG_WAITALL*/);
    if (n_read_this_time > 0) {
      n_read += n_read_this_time;
      if (n_read < len) {
        printf("Partial recv: %d of %d so far..\n", n_read, len);
      }
    } else {
      return n_read;
    }
    setsockopt(sock, IPPROTO_TCP, TCP_QUICKACK, (void *)&i, sizeof(i));

  } while (n_read < len);
  return n_read;
}

uint32_t proc_so_read(uint32_t offset) {
  uint32_t data = 0;
  uint16_t u16;

  fpga_ctrlr_rd(offset, &data);
  //u16 = *((uint16_t *) (virt_addr + offset));
  //data = (uint32_t)u16;
  return data;
}

void proc_so_write(uint32_t offset, uint32_t data) {
  uint16_t u16 = (uint16_t)data & 0xffff;

  fpga_ctrlr_wr(offset, data);
  //*((uint16_t *) (virt_addr + offset)) = u16;
  return;
}

void start_server(bool is_local_only) {
  reg_chnl = create_server(9001, is_local_only);
  if (-1 == reg_chnl) {
    /* Indicates bind/accept error in create server */
    printf("Socket already in use. Exiting the application\n");
    exit(1);
  }
}

/*****************************************************************
* proc_server
*
* This option provides a faster way to process I2C requests in
* support of the Credo eval boards.
* Credo supplies python scripts to configure their eval boards.
* We run these scripts on a switch CPU and connect the CP2112
* of that switch to the I2C pins of the eval board (using the
* CPU port QSFP channel).
*
* Because the driver is in python, the protocol is string based,
* using colons as separators.
*
* The command protocol is as follows:
*   Read: "r:<address>:<unused_data>"
*  Write: "w:<address>:<data>"
*
* E.g.
*   "r:0x9816:0x0000"
*   "w:0x9818:0x1234"
*
*****************************************************************/
#define CMD_LEN 15
int proc_server(void) {
  fpga_ctrlr_reset();
  while (true) {
    char cmd[CMD_LEN+1] = {0};
    uint32_t address, data;
    int rc, n_read, is_tile = false, is_grp = false, is_read = true;

    memset(cmd, 0, sizeof(cmd));
    n_read = read_from_socket(reg_chnl, (uint8_t *)cmd, CMD_LEN);
    if (0 && dbg_print) {
      int i;
      printf("\nsocket (%d bytes) => ", n_read);
      for (i = 0; i < n_read; i++) {
        printf("%02x ", cmd[i]);
      }
      printf("\n");
    }
    if (n_read != 15) return -1;

    cmd[8] = 0;
    address = strtoul(&cmd[2], NULL, 16);
    cmd[CMD_LEN] = 0;
    data = strtoul(&cmd[9], NULL, 16);
    if ((cmd[0] == 'w') || (cmd[0] == 'W')) is_read = false;
    else if ((cmd[0] == 'r') || (cmd[0] == 'R')) is_read = true;
    else if ((cmd[0] == 'g') || (cmd[0] == 'G')) {is_read = false; is_grp = true;}
    else if ((cmd[0] == 't') || (cmd[0] == 'T')) {is_read = false; is_tile = true;}
    else return -2;

    if (is_tile) {
      fpga_active_tile_set(address);
      //data = 0;
      data = address;
    } else if (is_grp) {
      active_group = address;
      //data = 0;
      data = active_group;
    } else if (is_read) {
      data = proc_so_read(address);
      snprintf(&cmd[9], 7, "0x%04x", data); // return data
    } else {
      uint32_t wr_data = data;
      proc_so_write(address, data);
      //data = 0;
      data = wr_data;
    }
    if (data > 0xffff) {
      printf(" Warn: data= %x\n", data);
      int i;
      printf("\nsocket (%d bytes) => ", n_read);
      for (i = 0; i < n_read; i++) {
        printf("%02x ", cmd[i]);
      }
      printf("\n");
    }
    // put back colon
    cmd[8] = ':';
    cmd[15] = 0;
    if (1 && dbg_print) {
      printf("Tile: %d : Group: %d : %s : Addr: %04x : Data : %04x\n",
             active_tile,
             active_group,
             is_tile ? " Tile" : 
             is_grp  ? "Group" : 
             is_read ? " Read" : "Write",
             address,
             data);
    }
    fflush(stdout);
    rc = write_to_socket(reg_chnl, (uint8_t *)cmd, CMD_LEN);
    if (rc != 0) return -4;
  }
  return reg_chnl;
}

int main(int argc, char **argv) {
    int fd;
    char *filename;
    off_t target, target_base;
    //int map_size = 4096UL;
    int map_size = (256ul*1024ul);

    if(argc < 1) {
        // pcimem /sys/bus/pci/devices/0001\:00\:07.0/resource0 0x100 w 0x00
        // argv[0]  [1]                                         [2]   [3] [4]
        fprintf(stderr, "\nUsage:\t%s <sysfile>\n"
            "\tsys file: sysfs file for the pci resource to act on\n",
	    argv[0]);
        exit(1);
    }
    //filename = argv[1];
    filename = "/sys/bus/pci/devices/0000:06:00.0/resource0";
    target = 0;

//int on_hw = false;
int on_hw = true;
if (on_hw) {

    if((fd = open(filename, O_RDWR | O_SYNC)) == -1) ERR_LOG;
    printf("%s opened.\n", filename);
    printf("Target offset is 0x%x, page size is %ld\n", (int) target, sysconf(_SC_PAGE_SIZE));
    fflush(stdout);

    //target_base = target & ~(sysconf(_SC_PAGE_SIZE)-1);
    target_base = 0;
    //if (target + 4 - target_base > map_size)
    // map_size = target + 4 - target_base;

    /* Map one page */
    printf("mmap(%d, %d, 0x%x, 0x%x, %d, 0x%x)\n", 0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (int) target);

    //map_base = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target_base);
    map_base = mmap(0, 256*1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target_base);
    if(map_base == (void *) -1) ERR_LOG;
    printf("PCI Memory mapped to address 0x%08lx.\n", (unsigned long) map_base);
    fflush(stdout);
} else {
    target_base = (off_t)0x0000;
    map_base = (void*)our_fake_address_space;
}

    // set virtual address
    virt_addr = map_base + target + target_base;
    fpga_ctrlr_base_address = (uintptr_t)virt_addr;
    fpga_ctrlr_set();
    fpga_ctrlr_reset();

    // process socket requests (forever)
    start_server();

    fflush(stdout);

    if(munmap(map_base, map_size) == -1) ERR_LOG;
    close(fd);
    return 0;
}


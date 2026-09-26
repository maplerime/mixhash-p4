import zlib
def raw(d,s): return zlib.crc32(d, s^0xffffffff)^0xffffffff
OP={0x06:28,0x07:12,0x08:12,0x09:12,0x04:12,0x0c:28}
def calc(ip):
    ihl=(ip[0]&0xf)*4; udp=ip[ihl:]; bth=udp[8:]
    hlen=OP.get(bth[0])
    if hlen is None: return None
    psh=bytearray(ip[:ihl]+udp[:8])
    psh[1]=0xff; psh[8]=0xff; psh[10:12]=b'\xff\xff'; psh[ihl+6:ihl+8]=b'\xff\xff'
    b2=bytearray(bth[:12]); b2[4]|=0xff
    crc=raw(bytes(psh)+bytes(b2),0xdebb20e3)
    crc=raw(bth[12:hlen],crc); crc=raw(bth[hlen:-4],crc)
    return ((~crc)&0xffffffff).to_bytes(4,'little')

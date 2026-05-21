# tna_arp_route

This example demonstrates ARP broadcast, L2 forwarding based on learned
MAC-IP-port bindings, and basic IPv4 routing on Tofino.

## Features

- **ARP Broadcast**: ARP requests for configured target IPs are flooded to
  all ports via a multicast group.
- **ARP Reply Unicast**: ARP replies are forwarded to a specific port.
- **SMAC Learning**: Records source MAC + source IP -> ingress port mappings.
  The control plane uses this to learn host locations.
- **L2 Forwarding**: Regular IP packets are forwarded based on destination MAC
  lookups in the dmac_table. Unknown MACs can be flooded via multicast.
- **IPv4 Routing**: IPv4 packets can be routed using LPM (Longest Prefix Match)
  with MAC rewrite and TTL decrement.

## Tables

| Table | Match Type | Key | Description |
|-------|-----------|-----|-------------|
| `SwitchIngress.smac_table` | exact | src MAC + src IP | Learn source host location |
| `SwitchIngress.dmac_table` | exact | dst MAC | Forward based on learned MAC-port |
| `SwitchIngress.arp_table` | exact | ARP opcode + target IP | ARP broadcast or unicast |
| `SwitchIngress.ipv4_route` | lpm | dst IP prefix | L3 routing with MAC rewrite |

## Processing Flow

```
Incoming packet
  ├─ ARP packet  → arp_table (broadcast or unicast)
  ├─ IPv4 packet
  │    ├─ smac_table  (learn src MAC/IP → port)
  │    └─ dmac_table  (forward via dst MAC → port)
  └─ Other → drop
```

## Running

```bash
export SDE=/path/to/sde/
export SDE_INSTALL=$SDE/install
sudo $SDE_INSTALL/bin/veth_setup.sh
```

Terminal 1:
```bash
cd $SDE
./run_tofino_model.sh --arch tofino -p tna_arp_route
```

Terminal 2:
```bash
cd $SDE
./run_switchd.sh --arch tofino -p tna_arp_route
```

Terminal 3:
```bash
cd $SDE
./run_p4_tests.sh --arch tofino -p tna_arp_route
```

## Test Cases

- **ArpBroadcastTest**: ARP request broadcast to all ports.
- **ArpReplyUnicastTest**: ARP reply unicast to specific port.
- **L2ForwardTest**: Regular IP packets forwarded based on learned MAC-port.
- **L2ForwardUnknownMacFloodTest**: Unknown MAC triggers flood.
- **Ipv4RouteTest**: IPv4 LPM routing with MAC rewrite and TTL decrement.
- **ArpAndL2ForwardCombinedTest**: Full flow - ARP learn, then L2 forward.

<!--
消えた声
代わりに告げる
さようなら
-->
# sACN Tern

This product has not been announced. If you are reading this, you were
snooping. I respect that.

## The problem

DMX lines have terminators. sACN does not. People will tell you that sACN is
a network protocol and that the PHY terminates the line. Those people are
describing the cable. Nobody is terminating the *stream*.

When an sACN source stops properly, it sends three packets with the
Stream_Terminated bit set (ANSI E1.31, section 6.2.6) and everybody knows it
has gone. When an sACN source is unplugged, or crashes, or is closed by
someone who wanted their laptop back, it sends nothing. Receivers wait out a
timeout and hope.

## The solution

The sACN Tern is a CH32V307 on your lighting network. It listens to every
sACN packet that reaches its port. It does not join any multicast groups; it
simply does not filter. For each source and universe it remembers the CID,
the source name, the priority, the sequence number and the IP address.

If a source goes quiet for three seconds, the Tern sends the three
Stream_Terminated packets the source should have sent. It sends them with the
source's CID, the source's name, the source's priority, the next three
sequence numbers, and the source's IP address. The MAC address is the Tern's
own. I felt one field should be honest.

It can do this for 25 sources at once. The 26th is on its own.

## Validation

The Tern will not terminate a stream it does not respect. Every field of
every packet is checked against the standard, one function per field, with
the section number in the comment. This includes:

- the Ethernet, IPv4 and UDP headers, both checksums, and that the multicast
  MAC, the multicast IP and the universe number all agree (9.3.1);
- every field of the root layer (5.1 to 5.6), including that the CID is a
  real RFC 4122 UUID with a version between 1 and 5;
- every field of the framing layer (6.1 to 6.2.7), including that the source
  name is null-terminated, valid UTF-8;
- every field of the DMP layer (7.1 to 7.7);
- the reserved option bits, which section 6.2.6 says receivers shall ignore.
  We do not ignore them.

A source that fails any check is not tracked, and so is never terminated. If
your console's CID is sixteen bytes somebody typed in, the Tern cannot help
you, and I would not want it to.

## Building

```
git clone --recursive <this repository>
cd firmware
make          # needs riscv64-unknown-elf-gcc with an rv32imac multilib
make check    # syntax only
```

It is built against WCH's own SDK and their raw-MAC example driver, with the
internal 10BASE-T PHY. Ten megabits is plenty for saying goodbye.

## Status

It compiles and links: 18.5 KB of flash. The packet logic has been run on a
desk computer against eight good frames and twenty-three bad ones, and against
its own termination packets, which it accepts. It has not been run on
hardware. It has not been run near a show.

The WCH SDK does not follow the coding standards and is forgiven. Read
`CONTRIBUTING.md` before you touch anything.

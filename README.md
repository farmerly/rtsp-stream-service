# RTSP-Stream-Service

## RTSP

RTSP(Real-TimeStream Protocol )是一种基于文本的应用层协议，一般默认端口为 554。

RTSP 1.0: [`RFC 2326`](https://www.rfc-editor.org/rfc/rfc2326)
RTSP 2.0: [`RFC 7826`](https://www.rfc-editor.org/rfc/rfc7826)

当使用 TCP 方式传输数据时，会使用 Interleaved Frame 来传输数据。Interleaved Frame 封装格式使用美元符号 $ 作为起始符（0x24）占一个字节，之后是一个字节的通道标识，然后是两个字节的数据包长度（网络字节序），最后是跟随的数据，如 RTP 包。

RTSP认证主要分两种，基本认证（basic authentication）和摘要认证（digest authentication）。

## SDP

SDP: [`RFC 2327`](https://www.rfc-editor.org/rfc/rfc2327)

Session 描述信息说明

```
v=  (protocol version)
o=  (owner/creator and session identifier).
s=  (session name)
i=* (session information)
u=* (URI of description)
e=* (email address)
p=* (phone number)
c=* (connection information - not required if included in all media)
b=* (bandwidth information)
One or more time descriptions (see below)
z=* (time zone adjustments)
k=* (encryption key)
a=* (zero or more session attribute lines)
Zero or more media descriptions (see below)
```

Time 描述信息说明

```
t=  (time the session is active)
r=* (zero or more repeat times)
```

Media 描述信息说明

```
m=  (media name and transport address)
i=* (media title)
c=* (connection information - optional if included at session-level)
b=* (bandwidth information)
k=* (encryption key)
a=* (zero or more media attribute lines)
```

下面是一个 SDP 描述信息示例：

```
v=0
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4
s=SDP Seminar
i=A Seminar on the session description protocol
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps
e=mjh@isi.edu (Mark Handley)
c=IN IP4 224.2.17.12/127
t=2873397496 2873404696
a=recvonly
m=audio 49170 RTP/AVP 0
m=video 51372 RTP/AVP 31
m=application 32416 udp wb
a=orient:portrait
```

```
o=<username> <session id> <version> <network type> <address type> <address>
```

## RTCP

[`RFC 3550`](https://www.rfc-editor.org/rfc/rfc3550) 定义了实时传输协议 RTP 和它的控制协议 RTCP。

协议定义五种 RTCP 报文：

| 类型 | 缩写 | 说明       |
| ---- | ---- | ---------- |
| 200  | SR   | 发送端报告 |
| 201  | RR   | 接收端报告 |
| 202  | SDES | 源端描述   |
| 203  | BYE  | 结束传输   |
| 204  | APP  | 特定应用   |

## RTP



# FFmpeg 命令使用

## 抓取视频流

```bash
ffmpeg -rtsp_transport tcp -i "rtsp://192.168.1.100:554/cam/realmonitor?channel=1&subtype=0" -vcodec copy -an video.mp4
```

## 推送视频流

```bash
ffmpeg -re -stream_loop -1 -i input.mp4 -c copy -f rtsp "rtsp://192.168.1.100:554/cam/realmonitor?channel=1&subtype=0"
```

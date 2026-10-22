"""Reference TCP text receiver; start explicitly on a trusted experiment network."""
import argparse
import math
import re
import socket

PACKET=re.compile(r'PKT:P=([-+\d.eE]+),R=([-+\d.eE]+),A=([-+\d.eE]+)')


def parse_packet(line):
    if len(line)>256:raise ValueError('Line exceeds 256 bytes')
    if line=='I am Esp8266':return {'type':'greeting'}
    m=PACKET.fullmatch(line)
    if not m:raise ValueError('Invalid telemetry frame')
    values=[float(value) for value in m.groups()]
    if not all(math.isfinite(value) for value in values):raise ValueError('Nonfinite sample')
    return dict(zip(('pitch','roll','accel'),values))


def serve(bind,port):
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as listener:
        listener.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        listener.bind((bind,port));listener.listen(1)
        print(f'Listening on {bind}:{port}; samples are uncalibrated accelerometer tilt.')
        while True:
            client,address=listener.accept()
            with client:
                client.settimeout(2);buffer=b''
                print('Connected:',address)
                while True:
                    try:data=client.recv(512)
                    except socket.timeout:
                        print('No new sample for 2 s');continue
                    except OSError:
                        break
                    if not data:break
                    buffer+=data
                    while b'\n' in buffer:
                        raw,buffer=buffer.split(b'\n',1)
                        try:packet=parse_packet(raw.rstrip(b'\r').decode('ascii'))
                        except (ValueError,UnicodeError) as exc:print('Rejected:',exc);continue
                        print(packet)
                        if packet.get('type')=='greeting':
                            try:client.sendall(b'Greeting accepted\n')
                            except OSError:break
                    if len(buffer)>256:
                        print('Closing peer with an oversized unterminated frame');break


if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--bind',default='0.0.0.0');p.add_argument('--port',type=int,default=8080);a=p.parse_args()
    try:serve(a.bind,a.port)
    except KeyboardInterrupt:pass

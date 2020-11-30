# Connector - 2014-2018

Ping tool for getting status of Minecraft servers.

## Usage

    Connector <host> [port [pack]]
`host` - domain or IPv4 of a server  to connect  
`port` - port, if not present defaults to 25565  
`pack` - sends whole connection in single chunk

Program outputs returned JSON onto `cout` stream.  
Other info and stats go onto separate `clog` stream.  

You can use pipes to redirect outputs  
to diffrent files or another program...

    Connector 2b2t.org > status.json
    Connector 2b2t.org > favicon server-icon.png
    Connector 2b2t.org 2>NUL 1> stripfavicon > prettyprint

However, you need to provide other programs on your own.

## History

"A very, very simple program I made over years,  
a predecessor of protomc library, never finished..."

It's a last version that was stored on disk,  
before I started using source version control.

Historical version, for view only...

## License

### ***This source code is under "Spaghetti license"***

"Feel free to use any part of this code  
for your own projects. Enjoy your spaghetti! ".

# [ellie] I started from this repo https://github.com/andrespp/docker-veins.git
# and made changes from there to get it to work on osx, include my changes,
# and install a few extra packages

# https://veins.car2x.org/tutorial/
# https://omnetpp.org/doc/omnetpp/InstallGuide.pdf
FROM ubuntu:16.04

MAINTAINER Andre Pereira andrespp@gmail.com

# Install dependencies
RUN apt-get update && apt-get install -y xauth unzip wget vim \
	build-essential gcc g++ bison flex perl tcl-dev tk-dev blt \
	libxml2-dev zlib1g-dev default-jre doxygen graphviz gdb \
	libwebkitgtk-1.0-0 openmpi-bin libopenmpi-dev libpcap-dev autoconf \
	automake libtool libproj-dev libgdal1-dev libfox-1.6-dev libgdal-dev \
	libxerces-c-dev qt4-dev-tools \
	qt5-default libqt5opengl5-dev openscenegraph \
	libopenscenegraph-dev openscenegraph-plugin-osgearth  osgearth \
	osgearth-data libosgearth-dev libgeos-dev software-properties-common \
	openssl libssl-dev && \
	add-apt-repository -y ppa:ubuntugis/ppa && \
	apt-get update && apt-get -y install libosgearth-dev

WORKDIR /root

# Build and Install SUMO
# http://sumo.dlr.de/wiki/Installing/Linux_Build
RUN wget https://downloads.sourceforge.net/project/sumo/sumo/version%200.32.0/sumo-src-0.32.0.tar.gz && \
	tar zxf sumo-src-0.32.0.tar.gz && \
	export SUMO_HOME="/root/sumo-0.32.0" && \
	cd sumo-0.32.0/ && \
	./configure && \
	make && \
	make install && \
	cd .. && rm -rf sumo*

# Build and Install OMNet++ IDE
COPY ./omnetpp-5.3-src-linux.tgz ./

RUN	tar zxvf omnetpp-5.3-src-linux.tgz && \
	rm omnetpp-5.3-src-linux.tgz && \
	cd /root/omnetpp-5.3 && \
	export PATH=$PATH:/root/omnetpp-5.3/bin && \
	./configure && \
	make

# Veins src code
COPY ./veins-4.7.1/ /root/veins-4.7.1
RUN ls -la /root/veins-4.7.1

RUN cp /usr/lib/x86_64-linux-gnu/libssl.so /root/veins-4.7.1/src/.
RUN cp /usr/lib/x86_64-linux-gnu/libcrypto.so /root/veins-4.7.1/src/.

COPY ./entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]

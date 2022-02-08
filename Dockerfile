FROM debian:buster

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    libsdl2-dev \
    libfreetype6-dev \
    zlib1g-dev \
    libpng-dev \
    libncurses5-dev \
    quilt \
    cmake \
    curl

COPY ./entrypoint.sh /entrypoint.sh
COPY ./png2cpcsprite.c /png2cpcsprite.c
COPY ./2cdt /2cdt
COPY ./cpcxfs /cpcxfs
COPY ./dskinfo /dskinfo

ENTRYPOINT [ "/entrypoint.sh" ]
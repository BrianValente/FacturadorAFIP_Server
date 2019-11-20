FROM archlinux

EXPOSE 6969

RUN pacman -Sy --noconfirm base-devel ninja gdb cups boost cmake mariadb-libs zlib openssl cairo gsoap tinyxml2

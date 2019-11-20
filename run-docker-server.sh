#!/bin/sh

docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -d -p 6969:6969 -v $PWD:/afip --rm afip:1.0 /bin/sh -c "cd /afip/docker_build; ./Afip;"
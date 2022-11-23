#!/bin/bash

set -e
make & ./pipex './libft/libft.h' 'cat' 'grep include' 'testfile'
printf "\n\n\n"

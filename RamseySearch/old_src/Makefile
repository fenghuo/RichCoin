################################################################################
# Makefile                                                                     #
#                                                                              #
# Description: This file contains the make rules for project.                  #
#                                                                              #
# Authors: Yuanyang Zhang <zhangyy1209@gmail.com>,                             #
#          Li Zheng <hellobuaazl@gmail.com>                                    #
#                                                                              #
################################################################################

default: search_ramsey

search_ramsey: search_ramsey.cpp helper.cpp helper.h
	g++ search_ramsey.cpp helper.cpp -o search_ramsey -Wall

clean:
	rm -rf search_ramsey *~

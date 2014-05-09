#!/bin/bash
function start
{
	cd ./bin/
	./server
}

function stop
{
	killall -9 server
}

function restart
{
	stop
	start
}

function stat
{
	ps -ef | grep server
}

case $1 in
	start) start ;;
	stop) stop ;;
	restart) restart ;;
	stat) stat ;;
esac

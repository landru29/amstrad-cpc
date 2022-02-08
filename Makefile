PWD:=$(shell pwd)

cpc.deb: entrypoint.sh Dockerfile
	docker build -t cpc . \
	&& docker run -v $(PWD):/out cpc

clean:
	rm -f *.deb
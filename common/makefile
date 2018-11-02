
TINCL=-I../stringx/include -I../linq/lib -I../common
TLIBD=-L../stringx/lib -L../common/lib
TLIBI=-lstringx -lcommon

target: ../$(year)/day$(day).cpp
	cd ../$(year) && \
	mkdir -p bin && \
	g++ -std=c++14 day$(day).cpp $(TLIBD) $(TINCL) $(TLIBI) -o ./bin/day$(day)

clean:
	cd ../$(year) &&
	rm -Rf bin/* || true

CSRC=md5.cpp
COBJ=md5.o
CNAME=common

common: $(CSRC)
	g++ -std=c++14 -c $(CSRC)
	@mkdir -p lib
	ar rcs lib/lib$(CNAME).a $(COBJ)
	@rm $(COBJ)

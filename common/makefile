
target: ../$(year)/day$(day).cpp
	cd ../$(year) && \
	mkdir -p bin && \
	g++ -std=c++14 day$(day).cpp -L../stringx/lib -I../stringx/include -I../linq/lib -I../common -lstringx -o ./bin/day$(day)

clean:
	cd ../$(year) &&
	rm -Rf bin/* || true

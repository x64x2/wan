OUTPUT_FOLDER=bin
FILENAME=ipam

all: build
 
build:
	go build -o ${OUTPUT_FOLDER}/${FILENAME} main.go

install:
	cp ${OUTPUT_FOLDER}/${FILENAME} /usr/local/bin/${FILENAME}

clean:
	go clean
	rm -r ${OUTPUT_FOLDER}/

package:
	echo ${version}
	mkdir -p ipam-${version}/usr/bin
	mkdir -p ipam-${version}/usr/share/bash-completion/completions
	mkdir -p ipam-${version}/DEBIAN
	echo "Maintainer: somto <x64x2@mail.ru>" > ipam-${version}/DEBIAN/control
	echo "Source: ipam" >> ipam-${version}/DEBIAN/control
	echo "Priority: optional" >> ipam-${version}/DEBIAN/control
	echo "Standards-Version: $(subst ~,.,${version})" >> ipam-${version}/DEBIAN/control
	echo "Section: utils" >> ipam-${version}/DEBIAN/control
	echo "Package: ipam" >> ipam-${version}/DEBIAN/control
	echo "Architecture: all" >> ipam-${version}/DEBIAN/control
	echo "Description: cli-based IPAM with PowerDNS integration" >> ipam-${version}/DEBIAN/control
	echo "Version: ${version}" >> ipam-${version}/DEBIAN/control
	echo "Homepage: https://sr.ht/~lauralani/ipam/" >> ipam-${version}/DEBIAN/control
	go build -o ${OUTPUT_FOLDER}/${FILENAME} main.go
	mv bin/ipam ipam-${version}/usr/bin
	ipam-${version}/usr/bin/ipam completion bash > ipam-${version}/usr/share/bash-completion/completions/ipam
	dpkg-deb -Z xz --build ipam-${version}/
	rm -rf ipam-${version}/

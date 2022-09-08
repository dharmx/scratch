all: link

run: link
	@echo ====THE PROGRAM STARTS FROM HERE====
	@_build/scratch

link: build
	ninja -C _build

build: format
	[ ! -d _build ] && meson _build || meson --reconfigure _build

format:
	clang-format -i **/*.c **/*.h

clean:
	rm -rf _build

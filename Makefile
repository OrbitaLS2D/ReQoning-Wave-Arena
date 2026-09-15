# ReQoning Wave Arena — run inside distrobox q3dev:
#   make          # engine + gamecode + pak + install
#   make menus    # only repack ui into rwapak0.pk3 (no compile)

PREFIX      ?= $(HOME)/RWA
ARCH        ?= x86_64
JOBS        ?= $(shell nproc)

ENGINE      := engine
ENGINE_OUT  := $(ENGINE)/build/release-linux-$(ARCH)
QVMDIR      := gamecode/build/linux-qvm
STAGE       := build/rwapak0
PK3         := build/rwapak0.pk3

BINS := reqwa.x64 reqwa.ded.x64 reqwa_opengl_$(ARCH).so reqwa_vulkan_$(ARCH).so

.PHONY: all engine gamecode pak menus dirs install clean help

all: engine gamecode pak install

help:
	@echo "make          - build everything and install to $(PREFIX)"
	@echo "make menus    - pack .menu files into rwapak0.pk3 (no compile)"
	@echo "make install  - copy bins + rwapak0.pk3 to $(PREFIX)"

engine:
	$(MAKE) -C $(ENGINE) -j$(JOBS)

gamecode:
	$(MAKE) -C $(QVMDIR) dirs vm/qagame.qvm vm/cgame.qvm vm/ui.qvm

pak: $(PK3)

$(PK3): gamecode
	@rm -rf $(STAGE)
	@mkdir -p $(STAGE)/vm
	@if [ -d gamecode/assets ]; then cp -a gamecode/assets/. $(STAGE)/; fi
	@if [ -d assets/ui ]; then mkdir -p $(STAGE)/ui && cp -a assets/ui/. $(STAGE)/ui/; fi
	@cp -a $(QVMDIR)/vm/*.qvm $(STAGE)/vm/
	@cp -a $(QVMDIR)/vm/*.jts $(STAGE)/vm/ 2>/dev/null || true
	@rm -f $(PK3)
	@cd $(STAGE) && zip -r -9 "$(CURDIR)/$(PK3)" .
	@echo "created $(PK3)"

# .menu only — uses QVMs already built
menus:
	@test -f $(QVMDIR)/vm/ui.qvm || { echo "run: make gamecode   first"; exit 1; }
	@rm -rf $(STAGE)
	@mkdir -p $(STAGE)/vm
	@if [ -d gamecode/assets ]; then cp -a gamecode/assets/. $(STAGE)/; fi
	@if [ -d assets/ui ]; then mkdir -p $(STAGE)/ui && cp -a assets/ui/. $(STAGE)/ui/; fi
	@cp -a $(QVMDIR)/vm/*.qvm $(STAGE)/vm/
	@cp -a $(QVMDIR)/vm/*.jts $(STAGE)/vm/ 2>/dev/null || true
	@rm -f $(PK3)
	@cd $(STAGE) && zip -r -9 "$(CURDIR)/$(PK3)" .
	@$(MAKE) install-pak
	@echo "rwapak0.pk3 updated"

dirs:
	mkdir -p $(PREFIX)/baserwa \
	         $(PREFIX)/idassets/baseq3 \
	         $(PREFIX)/idassets/missionpack \
	         $(PREFIX)/maps

install: dirs engine pak install-bins install-pak play.sh install-docs

install-bins: dirs
	@for f in $(BINS); do \
		if [ -f "$(ENGINE_OUT)/$$f" ]; then cp -v "$(ENGINE_OUT)/$$f" "$(PREFIX)/"; fi; \
	done

install-pak: dirs $(PK3)
	rm -f $(PREFIX)/baserwa/zz_ui.pk3 \
	      $(PREFIX)/baserwa/zzrwa.pk3 \
	      $(PREFIX)/baserwa/zz_rwa.pk3 \
	      $(PREFIX)/baserwa/pak3a.pk3 \
	      $(PREFIX)/baserwa/z_ta.pk3 \
	      $(PREFIX)/baserwa/rwapak1.pk3
	cp -v $(PK3) $(PREFIX)/baserwa/rwapak0.pk3
	@if [ -d assets/ui ]; then mkdir -p $(PREFIX)/baserwa/ui && cp -a assets/ui/. $(PREFIX)/baserwa/ui/; fi

install-docs: dirs
	@if [ -f INSTALL.txt ]; then cp -v INSTALL.txt $(PREFIX)/README.txt; else echo "NOTE: no INSTALL.txt"; fi

play.sh: dirs
	@printf '%s\n' \
	  '#!/bin/sh' \
	  'ROOT="$$(cd "$$(dirname "$$0")" && pwd)"' \
	  'exec "$$ROOT/reqwa.x64" +set fs_basepath "$$ROOT" +set fs_homepath "$$ROOT" "$$@"' \
	  > $(PREFIX)/play.sh
	chmod +x $(PREFIX)/play.sh
	@echo "wrote $(PREFIX)/play.sh"

clean:
	$(MAKE) -C $(ENGINE) clean
	$(MAKE) -C $(QVMDIR) clean
	rm -rf build

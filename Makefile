.PHONY: all

all:
	$(MAKE) npm_install
	$(MAKE) npm_build_bundle
	$(MAKE) install

.PHONY: npm_install
npm_install:
	cd client && npm install

.PHONY: npm_build_bundle
npm_build_bundle:
	cd client && ./node_modules/.bin/webpack --progress

.PHONY: install
install:
	R -q -e 'devtools::install("~/libs/httpgd")'

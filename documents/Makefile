.PHONY: deps start build serve deploy clean distclean help

help:
	@echo "XFEL Documentation"
	@echo "=================="
	@echo "  make deps      - Install dependencies"
	@echo "  make start     - Start dev server with hot-reload"
	@echo "  make build     - Production build"
	@echo "  make serve     - Build and serve locally"
	@echo "  make deploy    - Deploy to GitHub Pages"
	@echo "  make clean     - Remove build artifacts and cache"
	@echo "  make distclean - Remove all generated files including dependencies"

deps:
	npm install

start: deps
	npm start

build: deps
	npm run build

serve: build
	npm run serve

deploy:
	gh workflow run deploy.yml

clean:
	rm -rf build .docusaurus

distclean: clean
	rm -rf node_modules package-lock.json

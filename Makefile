# ==============================================================================
# Program:       Makefile
# Description:   C compiler
# Software/Tool: gcc
# ==============================================================================

binfile = mpp
cfiles = main.c mpp_config.c src/mpp_client.c src/mpp_ffmpeg.c src/mpp_notify.c
config = `pkgconf --cflags --libs libmpdclient libnotify libavformat MagickWand gdk-pixbuf-2.0 glib-2.0`
systempath = /usr/local/bin
userpath = ~/.local/bin

default:
	@echo "Targets: compile install remove uninstall debug clean"

compile:
	@echo "Compiling from $(cfiles)"
	@gcc ${config} -o $(binfile) $(cfiles)
	@echo "$(binfile) has been created"
	@echo "Done!"

remove:
	@echo "Removing files..."
	@rm "$(binfile)" 2> /dev/null || echo "No file to remove."
	@echo "Done!"

install-system:
	@echo "Installing in user bin path..."
	@gcc ${config} -o $(systempath)/$(binfile) $(cfiles)
	@echo "Done!"
	@echo "File created in $(systempath)/$(binfile)"

install-user:
	@echo "Installing in user bin path..."
	@gcc ${config} -o $(userpath)/$(binfile) $(cfiles)
	@echo "Done!"
	@echo "File created in $(userpath)/$(binfile)"

uninstall:
	@echo "Removing files..."
	@rm "$(userpath)/$(binfile)" | "$(systempath)/$(binfile)" 2> /dev/null || echo "No file to remove."
	@echo "Done!"

debug:
	@echo "Compiling from $(cfiles)"
	@gcc -c ${config} -S $(cfiles)

clean:
	@echo "Removing debug files."
	@rm *.s 2> /dev/null || echo "No files to remove."

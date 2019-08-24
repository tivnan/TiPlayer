

#include"stdafx.h"
#include <Windows.h>
#include "vlc/vlc.h"


int main(int argc, char **argv) {
	libvlc_instance_t *vlc;
	const char *url;

	const char *sout = "#transcode{vcodec=h264,fps=25,venc=x264{preset=ultrafast,"\
		"profile=baseline,tune=zerolatency},vb=512,"                              \
		"acodec=mpga,ab=64,channels=2}"                                           \
		":duplicate{dst=display,dst=standard{access=udp,mux=ts,dst=233.233.233.233:6666}}";
	const char *media_name = "Lei's test";


	url = "cuc_ieschool.flv";

	vlc = libvlc_new(0, NULL);
	libvlc_vlm_add_broadcast(vlc, media_name, url, sout, 0, NULL, true, false);
	libvlc_vlm_play_media(vlc, media_name);

	//play 30s
	_sleep(30000);

	libvlc_vlm_stop_media(vlc, media_name);
	libvlc_vlm_release(vlc);
	return 0;
}
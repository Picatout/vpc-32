/* 
 * File:   ntsc.h
 * Author: Jacques
 *
 * Created on 26 août 2013, 07:48
 */

#ifndef NTSC_H
#define	NTSC_H

#ifdef	__cplusplus
extern "C" {
#endif

#define VRES 230  // résolution verticale
#define HRES 320  // résolution horizontale

extern unsigned int video_bmp[VRES][HRES/32];
extern volatile unsigned int ln_cnt;
extern volatile unsigned int video;

// protoypes fonction interface
    void VideoInit(void);

#ifdef	__cplusplus
}
#endif

#endif	/* NTSC_H */


/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/vesa.h                                         */
/* file date : 2017/12/03                                                     */
/* file info : vesa video mode definitions                                    */
/* ************************************************************************** */
#ifndef VESA_H
#define VESA_H
#include <stdint.h>
#include <vesa-def.h>
#pragma pack(push, 1)

MODE_INFO *GetModeInfo(void);
VESA_INFO *GetVesaInfo(void);
void vesa_start(VESA_INFO *vesa, MODE_INFO *mode);

#pragma pack(pop)
#endif // VESA_H
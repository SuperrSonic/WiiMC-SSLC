/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#ifdef MP_DEBUG
#include <assert.h>
#endif

#include "mp_msg.h"
#include "help_mp.h"
#include "m_option.h"
#include "m_struct.h"


#include "img_format.h"
#include "mp_image.h"
#include "vf.h"

#include "libvo/fastmemcpy.h"
#include "libavutil/mem.h"

extern const vf_info_t vf_info_1bpp;
extern const vf_info_t vf_info_2xsai;
extern const vf_info_t vf_info_ass;
extern const vf_info_t vf_info_blackframe;
extern const vf_info_t vf_info_bmovl;
extern const vf_info_t vf_info_boxblur;
extern const vf_info_t vf_info_crop;
extern const vf_info_t vf_info_cropdetect;
extern const vf_info_t vf_info_decimate;
extern const vf_info_t vf_info_delogo;
extern const vf_info_t vf_info_denoise3d;
extern const vf_info_t vf_info_detc;
extern const vf_info_t vf_info_dint;
extern const vf_info_t vf_info_divtc;
extern const vf_info_t vf_info_down3dright;
extern const vf_info_t vf_info_dsize;
extern const vf_info_t vf_info_dvbscale;
extern const vf_info_t vf_info_eq2;
extern const vf_info_t vf_info_eq;
extern const vf_info_t vf_info_expand;
extern const vf_info_t vf_info_field;
extern const vf_info_t vf_info_fil;
extern const vf_info_t vf_info_filmdint;
extern const vf_info_t vf_info_fixpts;
extern const vf_info_t vf_info_flip;
extern const vf_info_t vf_info_format;
extern const vf_info_t vf_info_framestep;
extern const vf_info_t vf_info_fspp;
extern const vf_info_t vf_info_geq;
extern const vf_info_t vf_info_gradfun;
extern const vf_info_t vf_info_halfpack;
extern const vf_info_t vf_info_harddup;
extern const vf_info_t vf_info_hqdn3d;
extern const vf_info_t vf_info_hue;
extern const vf_info_t vf_info_il;
extern const vf_info_t vf_info_ilpack;
extern const vf_info_t vf_info_ivtc;
extern const vf_info_t vf_info_kerndeint;
extern const vf_info_t vf_info_lavc;
extern const vf_info_t vf_info_lavcdeint;
extern const vf_info_t vf_info_lavfi;
extern const vf_info_t vf_info_mcdeint;
extern const vf_info_t vf_info_mirror;
extern const vf_info_t vf_info_noformat;
extern const vf_info_t vf_info_noise;
extern const vf_info_t vf_info_ow;
extern const vf_info_t vf_info_palette;
extern const vf_info_t vf_info_perspective;
extern const vf_info_t vf_info_phase;
extern const vf_info_t vf_info_pp7;
//extern const vf_info_t vf_info_pp;
extern const vf_info_t vf_info_pullup;
extern const vf_info_t vf_info_qp;
extern const vf_info_t vf_info_rectangle;
extern const vf_info_t vf_info_remove_logo;
extern const vf_info_t vf_info_rgbtest;
extern const vf_info_t vf_info_rotate;
extern const vf_info_t vf_info_sab;
extern const vf_info_t vf_info_scale;
extern const vf_info_t vf_info_screenshot;
extern const vf_info_t vf_info_smartblur;
extern const vf_info_t vf_info_softpulldown;
extern const vf_info_t vf_info_softskip;
extern const vf_info_t vf_info_spp;
extern const vf_info_t vf_info_stereo3d;
extern const vf_info_t vf_info_swapuv;
extern const vf_info_t vf_info_telecine;
extern const vf_info_t vf_info_test;
extern const vf_info_t vf_info_tfields;
extern const vf_info_t vf_info_tile;
extern const vf_info_t vf_info_tinterlace;
extern const vf_info_t vf_info_unsharp;
extern const vf_info_t vf_info_uspp;
extern const vf_info_t vf_info_vo;
extern const vf_info_t vf_info_yadif;
extern const vf_info_t vf_info_yuvcsp;
extern const vf_info_t vf_info_yvu9;
extern const vf_info_t vf_info_zrmjpeg;

// list of available filters:
static const vf_info_t* const filter_list[]={
    &vf_info_rectangle,
#ifdef HAVE_POSIX_SELECT
    &vf_info_bmovl,
#endif
    &vf_info_crop,
    &vf_info_expand,
    &vf_info_scale,
//    &vf_info_osd,
    &vf_info_vo,
    &vf_info_format,
    &vf_info_noformat,
    &vf_info_flip,
    &vf_info_rotate,
    &vf_info_mirror,
    &vf_info_palette,
    &vf_info_pp7,
#ifdef CONFIG_FFMPEG
    &vf_info_lavc,
    &vf_info_lavcdeint,
#ifdef CONFIG_VF_LAVFI
    &vf_info_lavfi,
#endif
    &vf_info_screenshot,
    &vf_info_geq,
#endif
#ifdef CONFIG_POSTPROC
   // &vf_info_pp,
#endif
#ifdef CONFIG_ZR
    &vf_info_zrmjpeg,
#endif
    &vf_info_dvbscale,
    &vf_info_cropdetect,
    &vf_info_test,
    &vf_info_noise,
    &vf_info_yvu9,
    &vf_info_eq,
    &vf_info_eq2,
    &vf_info_gradfun,
    &vf_info_halfpack,
    &vf_info_dint,
    &vf_info_1bpp,
    &vf_info_2xsai,
    &vf_info_unsharp,
    &vf_info_swapuv,
    &vf_info_il,
    &vf_info_fil,
    &vf_info_boxblur,
    &vf_info_sab,
    &vf_info_smartblur,
    &vf_info_perspective,
    &vf_info_down3dright,
    &vf_info_field,
    &vf_info_denoise3d,
    &vf_info_hqdn3d,
    &vf_info_detc,
    &vf_info_telecine,
    &vf_info_tinterlace,
    &vf_info_tfields,
    &vf_info_ivtc,
    &vf_info_ilpack,
    &vf_info_dsize,
    &vf_info_decimate,
    &vf_info_softpulldown,
    &vf_info_pullup,
    &vf_info_filmdint,
    &vf_info_framestep,
    &vf_info_tile,
    &vf_info_delogo,
    &vf_info_remove_logo,
    &vf_info_hue,
#ifdef CONFIG_FFMPEG_A
    &vf_info_spp,
    &vf_info_uspp,
    &vf_info_fspp,
    &vf_info_qp,
    &vf_info_mcdeint,
#endif
    &vf_info_yuvcsp,
    &vf_info_kerndeint,
    &vf_info_rgbtest,
    &vf_info_phase,
    &vf_info_divtc,
    &vf_info_harddup,
    &vf_info_softskip,
#ifdef CONFIG_ASS
    &vf_info_ass,
#endif
    &vf_info_yadif,
    &vf_info_blackframe,
    &vf_info_ow,
    &vf_info_fixpts,
    &vf_info_stereo3d,
    NULL
};

// For the vf option
m_obj_settings_t* vf_settings = NULL;
const m_obj_list_t vf_obj_list = {
  (void**)filter_list,
  M_ST_OFF(vf_info_t,name),
  M_ST_OFF(vf_info_t,info),
  M_ST_OFF(vf_info_t,opts)
};

//============================================================================
// mpi stuff:

void vf_mpi_clear(mp_image_t* mpi,int x0,int y0,int w,int h){
    int y;
    if(mpi->flags&MP_IMGFLAG_PLANAR){
        y0&=~1;h+=h&1;
        if(x0==0 && w==mpi->width){
            // full width clear:
            memset(mpi->planes[0]+mpi->stride[0]*y0,0,mpi->stride[0]*h);
            memset(mpi->planes[1]+mpi->stride[1]*(y0>>mpi->chroma_y_shift),128,mpi->stride[1]*(h>>mpi->chroma_y_shift));
            memset(mpi->planes[2]+mpi->stride[2]*(y0>>mpi->chroma_y_shift),128,mpi->stride[2]*(h>>mpi->chroma_y_shift));
        } else
        for(y=y0;y<y0+h;y+=2){
            memset(mpi->planes[0]+x0+mpi->stride[0]*y,0,w);
            memset(mpi->planes[0]+x0+mpi->stride[0]*(y+1),0,w);
            memset(mpi->planes[1]+(x0>>mpi->chroma_x_shift)+mpi->stride[1]*(y>>mpi->chroma_y_shift),128,(w>>mpi->chroma_x_shift));
            memset(mpi->planes[2]+(x0>>mpi->chroma_x_shift)+mpi->stride[2]*(y>>mpi->chroma_y_shift),128,(w>>mpi->chroma_x_shift));
        }
        return;
    }
    // packed:
    for(y=y0;y<y0+h;y++){
        unsigned char* dst=mpi->planes[0]+mpi->stride[0]*y+(mpi->bpp>>3)*x0;
        if(mpi->flags&MP_IMGFLAG_YUV){
            unsigned int* p=(unsigned int*) dst;
            int size=(mpi->bpp>>3)*w/4;
            int i;
#if HAVE_BIGENDIAN
#define CLEAR_PACKEDYUV_PATTERN 0x00800080
#define CLEAR_PACKEDYUV_PATTERN_SWAPPED 0x80008000
#else
#define CLEAR_PACKEDYUV_PATTERN 0x80008000
#define CLEAR_PACKEDYUV_PATTERN_SWAPPED 0x00800080
#endif
            if(mpi->flags&MP_IMGFLAG_SWAPPED){
                for(i=0;i<size-3;i+=4) p[i]=p[i+1]=p[i+2]=p[i+3]=CLEAR_PACKEDYUV_PATTERN_SWAPPED;
                for(;i<size;i++) p[i]=CLEAR_PACKEDYUV_PATTERN_SWAPPED;
            } else {
                for(i=0;i<size-3;i+=4) p[i]=p[i+1]=p[i+2]=p[i+3]=CLEAR_PACKEDYUV_PATTERN;
                for(;i<size;i++) p[i]=CLEAR_PACKEDYUV_PATTERN;
            }
        } else
            memset(dst,0,(mpi->bpp>>3)*w);
    }
}

mp_image_t* vf_get_image(vf_instance_t* vf, unsigned int outfmt, int mp_imgtype, int mp_imgflag, int w, int h){
  mp_image_t* mpi=NULL;
  int w2;
  int number = (mp_imgtype >> 16) - 1;

#ifdef MP_DEBUG
  assert(w == -1 || w >= vf->w);
  assert(h == -1 || h >= vf->h);
  assert(vf->w > 0);
  assert(vf->h > 0);
#endif

//  fprintf(stderr, "get_image: %d:%d, vf: %d:%d\n", w,h,vf->w,vf->h);

  if (w == -1) w = vf->w;
  if (h == -1) h = vf->h;

  w2=(mp_imgflag&MP_IMGFLAG_ACCEPT_ALIGNED_STRIDE)?FFALIGN(w, 32):w;

  if(vf->put_image==vf_next_put_image){
      // passthru mode, if the filter uses the fallback/default put_image() code
      return vf_get_image(vf->next,outfmt,mp_imgtype,mp_imgflag,w,h);
  }

  // Note: we should call libvo first to check if it supports direct rendering
  // and if not, then fallback to software buffers:
  switch(mp_imgtype & 0xff){
  case MP_IMGTYPE_EXPORT:
    if(!vf->imgctx.export_images[0]) vf->imgctx.export_images[0]=new_mp_image(w2,h);
    mpi=vf->imgctx.export_images[0];
    break;
  case MP_IMGTYPE_STATIC:
    if(!vf->imgctx.static_images[0]) vf->imgctx.static_images[0]=new_mp_image(w2,h);
    mpi=vf->imgctx.static_images[0];
    break;
  case MP_IMGTYPE_TEMP:
    if(!vf->imgctx.temp_images[0]) vf->imgctx.temp_images[0]=new_mp_image(w2,h);
    mpi=vf->imgctx.temp_images[0];
    break;
  case MP_IMGTYPE_IPB:
    if(!(mp_imgflag&MP_IMGFLAG_READABLE)){ // B frame:
      if(!vf->imgctx.temp_images[0]) vf->imgctx.temp_images[0]=new_mp_image(w2,h);
      mpi=vf->imgctx.temp_images[0];
      break;
    }
  case MP_IMGTYPE_IP:
    if(!vf->imgctx.static_images[vf->imgctx.static_idx]) vf->imgctx.static_images[vf->imgctx.static_idx]=new_mp_image(w2,h);
    mpi=vf->imgctx.static_images[vf->imgctx.static_idx];
    vf->imgctx.static_idx^=1;
    break;
  case MP_IMGTYPE_NUMBERED:
    if (number == -1) {
      int i;
      for (i = 0; i < NUM_NUMBERED_MPI; i++)
        if (!vf->imgctx.numbered_images[i] || !vf->imgctx.numbered_images[i]->usage_count)
          break;
      number = i;
    }
    if (number < 0 || number >= NUM_NUMBERED_MPI) {
      mp_msg(MSGT_VFILTER, MSGL_FATAL, "Ran out of numbered images, expect crash. Filter before %s is broken.\n", vf->info->name);
      return NULL;
    }
    if (!vf->imgctx.numbered_images[number]) vf->imgctx.numbered_images[number] = new_mp_image(w2,h);
    mpi = vf->imgctx.numbered_images[number];
    mpi->number = number;
    break;
  }
  if(mpi){
    int missing_palette = !(mpi->flags & MP_IMGFLAG_RGB_PALETTE) && (mp_imgflag & MP_IMGFLAG_RGB_PALETTE);
    mpi->type=mp_imgtype;
    mpi->w=vf->w; mpi->h=vf->h;
    // keep buffer allocation status & color flags only:
//  mpi->flags&=~(MP_IMGFLAG_PRESERVE|MP_IMGFLAG_READABLE|MP_IMGFLAG_DIRECT);
    mpi->flags&=MP_IMGFLAG_ALLOCATED|MP_IMGFLAG_TYPE_DISPLAYED|MP_IMGFLAGMASK_COLORS;
    // accept restrictions, draw_slice and palette flags only:
    mpi->flags|=mp_imgflag&(MP_IMGFLAGMASK_RESTRICTIONS|MP_IMGFLAG_DRAW_CALLBACK|MP_IMGFLAG_RGB_PALETTE);
    if(!vf->draw_slice) mpi->flags&=~MP_IMGFLAG_DRAW_CALLBACK;
    if(mpi->width!=w2 || mpi->height!=h || missing_palette){
//      printf("vf.c: MPI parameters changed!  %dx%d -> %dx%d   \n", mpi->width,mpi->height,w2,h);
        if(mpi->flags&MP_IMGFLAG_ALLOCATED){
            if(mpi->width<w2 || mpi->height<h || missing_palette){
                // need to re-allocate buffer memory:
                av_freep(&mpi->planes[0]);
                if (mpi->flags & MP_IMGFLAG_RGB_PALETTE)
                    av_freep(&mpi->planes[1]);
                mpi->flags&=~MP_IMGFLAG_ALLOCATED;
                mp_msg(MSGT_VFILTER,MSGL_V,"vf.c: have to REALLOCATE buffer memory in vf_%s :(\n",
                       vf->info->name);
            }
//      } else {
        } {
            mpi->width=w2; mpi->chroma_width=(w2 + (1<<mpi->chroma_x_shift) - 1)>>mpi->chroma_x_shift;
            mpi->height=h; mpi->chroma_height=(h + (1<<mpi->chroma_y_shift) - 1)>>mpi->chroma_y_shift;
        }
    }
    if(!mpi->bpp) mp_image_setfmt(mpi,outfmt);
    if(!(mpi->flags&MP_IMGFLAG_ALLOCATED) && mpi->type>MP_IMGTYPE_EXPORT){

        // check libvo first!
        if(vf->get_image) vf->get_image(vf,mpi);

        if(!(mpi->flags&MP_IMGFLAG_DIRECT)){
          // non-direct and not yet allocated image. allocate it!
          if (!mpi->bpp) { // no way we can allocate this
              mp_msg(MSGT_DECVIDEO, MSGL_FATAL,
                     "vf_get_image: Tried to allocate a format that can not be allocated!\n");
              return NULL;
          }

          // check if codec prefer aligned stride:
          if(mp_imgflag&MP_IMGFLAG_PREFER_ALIGNED_STRIDE){
              int align=(mpi->flags&MP_IMGFLAG_PLANAR &&
                         mpi->flags&MP_IMGFLAG_YUV) ?
                         (16<<mpi->chroma_x_shift) : 32; // -- maybe FIXME
              w2=FFALIGN(w, align);
              if(mpi->width!=w2){
                  // we have to change width... check if we CAN co it:
                  int flags=vf->query_format(vf,outfmt); // should not fail
                  if(!(flags&3)) mp_msg(MSGT_DECVIDEO,MSGL_WARN,"??? vf_get_image{vf->query_format(outfmt)} failed!\n");
//                  printf("query -> 0x%X    \n",flags);
                  if(flags&VFCAP_ACCEPT_STRIDE){
                      mpi->width=w2;
                      mpi->chroma_width=(w2 + (1<<mpi->chroma_x_shift) - 1)>>mpi->chroma_x_shift;
                  }
              }
          }

          mp_image_alloc_planes(mpi);
//        printf("clearing img!\n");
          vf_mpi_clear(mpi,0,0,mpi->width,mpi->height);
        }
    }
    if(mpi->flags&MP_IMGFLAG_DRAW_CALLBACK)
        if(vf->start_slice) vf->start_slice(vf,mpi);
    if(!(mpi->flags&MP_IMGFLAG_TYPE_DISPLAYED)){
            mp_msg(MSGT_DECVIDEO,MSGL_V,"*** [%s] %s%s mp_image_t, %dx%dx%dbpp %s %s, %d bytes\n",
                  vf->info->name,
                  (mpi->type==MP_IMGTYPE_EXPORT)?"Exporting":
                  ((mpi->flags&MP_IMGFLAG_DIRECT)?"Direct Rendering":"Allocating"),
                  (mpi->flags&MP_IMGFLAG_DRAW_CALLBACK)?" (slices)":"",
                  mpi->width,mpi->height,mpi->bpp,
                  (mpi->flags&MP_IMGFLAG_YUV)?"YUV":((mpi->flags&MP_IMGFLAG_SWAPPED)?"BGR":"RGB"),
                  (mpi->flags&MP_IMGFLAG_PLANAR)?"planar":"packed",
                  mpi->bpp*mpi->width*mpi->height/8);
            mp_msg(MSGT_DECVIDEO,MSGL_DBG2,"(imgfmt: %x, planes: %p,%p,%p strides: %d,%d,%d, chroma: %dx%d, shift: h:%d,v:%d)\n",
                mpi->imgfmt, mpi->planes[0], mpi->planes[1], mpi->planes[2],
                mpi->stride[0], mpi->stride[1], mpi->stride[2],
                mpi->chroma_width, mpi->chroma_height, mpi->chroma_x_shift, mpi->chroma_y_shift);
            mpi->flags|=MP_IMGFLAG_TYPE_DISPLAYED;
    }

  mpi->qscale = NULL;
  }
  mpi->usage_count++;
  // TODO: figure out what is going on with EXPORT types
  if (mpi->usage_count > 1 && mpi->type != MP_IMGTYPE_EXPORT)
      mp_msg(MSGT_VFILTER, MSGL_V, "Suspicious mp_image usage count %i in vf_%s (type %i)\n",
             mpi->usage_count, vf->info->name, mpi->type);
//  printf("\rVF_MPI: %p %p %p %d %d %d    \n",
//      mpi->planes[0],mpi->planes[1],mpi->planes[2],
//      mpi->stride[0],mpi->stride[1],mpi->stride[2]);
  return mpi;
}

//============================================================================

// By default vf doesn't accept MPEGPES
static int vf_default_query_format(struct vf_instance *vf, unsigned int fmt){
  if(fmt == IMGFMT_MPEGPES) return 0;
  return vf_next_query_format(vf,fmt);
}

vf_instance_t* vf_open_plugin(const vf_info_t* const* filter_list, vf_instance_t* next, const char *name, char **args){
    vf_instance_t* vf;
    int i;
    for(i=0;;i++){
        if(!filter_list[i]){
            mp_msg(MSGT_VFILTER,MSGL_ERR,MSGTR_CouldNotFindVideoFilter,name);
            return NULL; // no such filter!
        }
        if(!strcmp(filter_list[i]->name,name)) break;
    }
    vf=malloc(sizeof(vf_instance_t));
    memset(vf,0,sizeof(vf_instance_t));
    vf->info=filter_list[i];
    vf->next=next;
    vf->config=vf_next_config;
    vf->control=vf_next_control;
    vf->query_format=vf_default_query_format;
    vf->put_image=vf_next_put_image;
    vf->default_caps=VFCAP_ACCEPT_STRIDE;
    vf->default_reqs=0;
    if(vf->info->opts) { // vf_vo get some special argument
      const m_struct_t* st = vf->info->opts;
      void* vf_priv = m_struct_alloc(st);
      int n;
      for(n = 0 ; args && args[2*n] ; n++)
        m_struct_set(st,vf_priv,args[2*n],args[2*n+1]);
      vf->priv = vf_priv;
      args = NULL;
    } else // Otherwise we should have the '_oldargs_'
      if(args && !strcmp(args[0],"_oldargs_"))
        args = (char**)args[1];
      else
        args = NULL;
    if(vf->info->vf_open(vf,(char*)args)>0) return vf; // Success!
    free(vf);
    mp_msg(MSGT_VFILTER,MSGL_ERR,MSGTR_CouldNotOpenVideoFilter,name);
    return NULL;
}

vf_instance_t* vf_open_filter(vf_instance_t* next, const char *name, char **args){
  if(args && strcmp(args[0],"_oldargs_")) {
    int i,l = 0;
    for(i = 0 ; args && args[2*i] ; i++)
      l += 1 + strlen(args[2*i]) + 1 + strlen(args[2*i+1]);
    l += strlen(name);
    {
      char str[l+1];
      char* p = str;
      p += sprintf(str,"%s",name);
      for(i = 0 ; args && args[2*i] ; i++)
        p += sprintf(p," %s=%s",args[2*i],args[2*i+1]);
      mp_msg(MSGT_VFILTER,MSGL_INFO,MSGTR_OpeningVideoFilter "[%s]\n",str);
    }
  } else if(strcmp(name,"vo")) {
    if(args && strcmp(args[0],"_oldargs_") == 0)
      mp_msg(MSGT_VFILTER,MSGL_INFO,MSGTR_OpeningVideoFilter
             "[%s=%s]\n", name,args[1]);
    else
      mp_msg(MSGT_VFILTER,MSGL_INFO,MSGTR_OpeningVideoFilter
             "[%s]\n", name);
  }
  return vf_open_plugin(filter_list,next,name,args);
}

/**
 * \brief adds a filter before the last one (which should be the vo filter).
 * \param vf start of the filter chain.
 * \param name name of the filter to add.
 * \param args argument list for the filter.
 * \return pointer to the filter instance that was created.
 */
vf_instance_t* vf_add_before_vo(vf_instance_t **vf, char *name, char **args) {
  vf_instance_t *vo, *prev = NULL, *new;
  // Find the last filter (should be vf_vo)
  for (vo = *vf; vo->next; vo = vo->next)
    prev = vo;
  new = vf_open_filter(vo, name, args);
  if (prev)
    prev->next = new;
  else
    *vf = new;
  return new;
}

//============================================================================

unsigned int vf_match_csp(vf_instance_t** vfp,const unsigned int* list,unsigned int preferred){
    vf_instance_t* vf=*vfp;
    const unsigned int* p;
    unsigned int best=0;
    int ret;
    if((p=list)) while(*p){
        ret=vf->query_format(vf,*p);
        mp_msg(MSGT_VFILTER,MSGL_V,"[%s] query(%s) -> %d\n",vf->info->name,vo_format_name(*p),ret&3);
        if(ret&2){ best=*p; break;} // no conversion -> bingo!
        if(ret&1 && !best) best=*p; // best with conversion
        ++p;
    }
    if(best) return best; // bingo, they have common csp!
    // ok, then try with scale:
    if(vf->info == &vf_info_scale) return 0; // avoid infinite recursion!
    vf=vf_open_filter(vf,"scale",NULL);
    if(!vf) return 0; // failed to init "scale"
    // try the preferred csp first:
    if(preferred && vf->query_format(vf,preferred)) best=preferred; else
    // try the list again, now with "scaler" :
    if((p=list)) while(*p){
        ret=vf->query_format(vf,*p);
        mp_msg(MSGT_VFILTER,MSGL_V,"[%s] query(%s) -> %d\n",vf->info->name,vo_format_name(*p),ret&3);
        if(ret&2){ best=*p; break;} // no conversion -> bingo!
        if(ret&1 && !best) best=*p; // best with conversion
        ++p;
    }
    if(best) *vfp=vf; // else uninit vf  !FIXME!
    return best;
}

void vf_clone_mpi_attributes(mp_image_t* dst, mp_image_t* src){
    dst->pict_type= src->pict_type;
    dst->fields = src->fields;
    dst->qscale_type= src->qscale_type;
    if(dst->width == src->width && dst->height == src->height){
        dst->qstride= src->qstride;
        dst->qscale= src->qscale;
    }
}

void vf_queue_frame(vf_instance_t *vf, int (*func)(vf_instance_t *))
{
    vf->continue_buffered_image = func;
}

// Output the next buffered image (if any) from the filter chain.
// The queue could be kept as a simple stack/list instead avoiding the
// looping here, but there's currently no good context variable where
// that could be stored so this was easier to implement.

int vf_output_queued_frame(vf_instance_t *vf)
{
    while (1) {
        int ret;
        vf_instance_t *current;
        vf_instance_t *last=NULL;
        int (*tmp)(vf_instance_t *);
        for (current = vf; current; current = current->next)
            if (current->continue_buffered_image)
                last = current;
        if (!last)
            return 0;
        tmp = last->continue_buffered_image;
        last->continue_buffered_image = NULL;
        ret = tmp(last);
        if (ret > 0) {
            vf->control(vf, VFCTRL_DRAW_OSD, NULL);
#ifdef CONFIG_ASS
            vf->control(vf, VFCTRL_DRAW_EOSD, NULL);
#endif
        }
        if (ret)
            return ret;
    }
}


/**
 * \brief Video config() function wrapper
 *
 * Blocks config() calls with different size or format for filters
 * with VFCAP_CONSTANT
 *
 * First call is redirected to vf->config.
 *
 * In following calls, it verifies that the configuration parameters
 * are unchanged, and returns either success or error.
 *
*/
int vf_config_wrapper(struct vf_instance *vf,
                    int width, int height, int d_width, int d_height,
                    unsigned int flags, unsigned int outfmt)
{
    int r;
    if ((vf->default_caps&VFCAP_CONSTANT) && vf->fmt.have_configured) {
        if ((vf->fmt.orig_width != width)
            || (vf->fmt.orig_height != height)
            || (vf->fmt.orig_fmt != outfmt)) {
            mp_msg(MSGT_VFILTER,MSGL_ERR,MSGTR_ResolutionDoesntMatch);
            return 0;
        }
        return 1;
    }
    vf->fmt.have_configured = 1;
    vf->fmt.orig_height = height;
    vf->fmt.orig_width = width;
    vf->fmt.orig_fmt = outfmt;
    r = vf->config(vf, width, height, d_width, d_height, flags, outfmt);
    if (!r) vf->fmt.have_configured = 0;
    return r;
}

int vf_next_config(struct vf_instance *vf,
        int width, int height, int d_width, int d_height,
        unsigned int voflags, unsigned int outfmt){
    int miss;
    int flags=vf->next->query_format(vf->next,outfmt);
    if(!flags){
        // hmm. colorspace mismatch!!!
        // let's insert the 'scale' filter, it does the job for us:
        vf_instance_t* vf2;
        if(vf->next->info==&vf_info_scale) return 0; // scale->scale
        vf2=vf_open_filter(vf->next,"scale",NULL);
        if(!vf2) return 0; // shouldn't happen!
        vf->next=vf2;
        flags=vf->next->query_format(vf->next,outfmt);
        if(!flags){
            mp_msg(MSGT_VFILTER,MSGL_ERR,MSGTR_CannotFindColorspace);
            return 0; // FAIL
        }
    }
    mp_msg(MSGT_VFILTER,MSGL_V,"REQ: flags=0x%X  req=0x%X  \n",flags,vf->default_reqs);
    miss=vf->default_reqs - (flags&vf->default_reqs);
    if(miss&VFCAP_ACCEPT_STRIDE){
        // vf requires stride support but vf->next doesn't support it!
        // let's insert the 'expand' filter, it does the job for us:
        vf_instance_t* vf2=vf_open_filter(vf->next,"expand",NULL);
        if(!vf2) return 0; // shouldn't happen!
        vf->next=vf2;
    }
    vf->next->w = width; vf->next->h = height;
    return vf_config_wrapper(vf->next,width,height,d_width,d_height,voflags,outfmt);
}

int vf_next_control(struct vf_instance *vf, int request, void* data){
    return vf->next->control(vf->next,request,data);
}

void vf_extra_flip(struct vf_instance *vf) {
    vf_next_control(vf, VFCTRL_DRAW_OSD, NULL);
#ifdef CONFIG_ASS
    vf_next_control(vf, VFCTRL_DRAW_EOSD, NULL);
#endif
    vf_next_control(vf, VFCTRL_FLIP_PAGE, NULL);
}

int vf_next_query_format(struct vf_instance *vf, unsigned int fmt){
    int flags=vf->next->query_format(vf->next,fmt);
    if(flags) flags|=vf->default_caps;
    return flags;
}

int vf_next_put_image(struct vf_instance *vf,mp_image_t *mpi, double pts){
    mpi->usage_count--;
    if (mpi->usage_count < 0) {
        mp_msg(MSGT_VFILTER, MSGL_V, "Bad mp_image usage count %i in vf_%s (type %i)\n",
               mpi->usage_count, vf->info->name, mpi->type);
        mpi->usage_count = 0;
    }
    return vf->next->put_image(vf->next,mpi, pts);
}

void vf_next_draw_slice(struct vf_instance *vf,unsigned char** src, int * stride,int w, int h, int x, int y){
    if (vf->next->draw_slice) {
        vf->next->draw_slice(vf->next,src,stride,w,h,x,y);
        return;
    }
    if (!vf->dmpi) {
        mp_msg(MSGT_VFILTER,MSGL_ERR,"draw_slice: dmpi not stored by vf_%s\n", vf->info->name);
        return;
    }
    if (!(vf->dmpi->flags & MP_IMGFLAG_PLANAR)) {
        memcpy_pic(vf->dmpi->planes[0]+y*vf->dmpi->stride[0]+vf->dmpi->bpp/8*x,
            src[0], vf->dmpi->bpp/8*w, h, vf->dmpi->stride[0], stride[0]);
        return;
    }
    memcpy_pic(vf->dmpi->planes[0]+y*vf->dmpi->stride[0]+x, src[0],
        w, h, vf->dmpi->stride[0], stride[0]);
    memcpy_pic(vf->dmpi->planes[1]+(y>>vf->dmpi->chroma_y_shift)*vf->dmpi->stride[1]+(x>>vf->dmpi->chroma_x_shift),
        src[1], w>>vf->dmpi->chroma_x_shift, h>>vf->dmpi->chroma_y_shift, vf->dmpi->stride[1], stride[1]);
    memcpy_pic(vf->dmpi->planes[2]+(y>>vf->dmpi->chroma_y_shift)*vf->dmpi->stride[2]+(x>>vf->dmpi->chroma_x_shift),
        src[2], w>>vf->dmpi->chroma_x_shift, h>>vf->dmpi->chroma_y_shift, vf->dmpi->stride[2], stride[2]);
}

//============================================================================

vf_instance_t* append_filters(vf_instance_t* last){
  vf_instance_t* vf;
  int i;

  if(vf_settings) {
    // We want to add them in the 'right order'
    for(i = 0 ; vf_settings[i].name ; i++)
      /* NOP */;
    for(i-- ; i >= 0 ; i--) {
      //printf("Open filter %s\n",vf_settings[i].name);
      vf = vf_open_filter(last,vf_settings[i].name,vf_settings[i].attribs);
      if(vf) last=vf;
    }
  }
  return last;
}

//============================================================================

void vf_uninit_filter(vf_instance_t* vf){
    if(vf->uninit) vf->uninit(vf);
    free_mp_image(vf->imgctx.static_images[0]);
    free_mp_image(vf->imgctx.static_images[1]);
    free_mp_image(vf->imgctx.temp_images[0]);
    free_mp_image(vf->imgctx.export_images[0]);
    free(vf);
}

void vf_uninit_filter_chain(vf_instance_t* vf){
    while(vf){
        vf_instance_t* next=vf->next;
        vf_uninit_filter(vf);
        vf=next;
    }
}

#include <ui/Surface.h>

#include <cassert>

#include <math/MathUtil.h>
#include <ui/SDL_image.h>
#include <util/Asset.h>


Surface::Surface(SDL_Surface& surface)
  : log_("Surface"),
    surface_(&surface),
    filename_(""),
    textureId_(0),
    clip_(0,0,surface.w,surface.h)
{
}

Surface::Surface()
  : log_("Surface"),
    surface_(nullptr),
    filename_(""),
    textureId_(0),
    clip_(0,0,0,0)
{
}


Surface::~Surface()
{
  releaseResources();
}


void Surface::releaseResources()
{
  if (textureId_) {
    glDeleteTextures(1, &textureId_);
    textureId_ = 0;
  }
}


void Surface::loadImage(const std::string& filename)
{
  const std::string path {Asset::path(Asset::IMAGE, filename)};
  log_.d() << "Loading image: " << path << Log::end;

  releaseResources();

  surface_.reset(IMG_Load(path.c_str()));
  if (!surface_) {
    log_.e() << "Failed to load image " << path << Log::end;
    log_.e() << SDL_GetError() << Log::end;
  }
  filename_ = filename;

  setClip(Rect(0, 0, getWidth(), getHeight()));
}

void Surface::setSurface(SDL_Surface& surface)
{
  surface_.reset(&surface);
  releaseResources();
  setClip(Rect(0, 0, getWidth(), getHeight()));
}


unsigned int Surface::getWidth() const
{
  assert(surface_ && "Surface not set!");
  return surface_->w;
}

unsigned int Surface::getHeight() const
{
  assert(surface_ && "Surface not set!");
  return surface_->h;
}

const Rect& Surface::getClip() const
{return clip_;}
void Surface::setClip(const Rect& clip)
{clip_ = clip;}


float Surface::glTexCoordX(float texcoord) const
{
  float surfSpace = ((float)clip_.x() + texcoord * (float)clip_.w())/(float)getWidth();
  return MathUtil::nextPow2TexCoord(surfSpace, getWidth());
}


float Surface::glTexCoordY(float texcoord) const
{
  float surfSpace = ((float)clip_.y() + texcoord * (float)clip_.h())/(float)getHeight();
  return MathUtil::nextPow2TexCoord(surfSpace, getHeight());
}


void Surface::prepareForGl()
{
  assert(surface_ && "Surface not set!");
  if (textureId_) {return;}

  const int width  = MathUtil::nextPow2(surface_->w);
  const int height = MathUtil::nextPow2(surface_->h);

  int bpp = 0;
  Uint32 Rmask, Gmask, Bmask, Amask = 0;
  SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp,
                             &Rmask, &Gmask, &Bmask, &Amask);
  SurfacePtr tempSurface(SDL_CreateRGBSurface(0, width, height,
                                              bpp,
                                              Rmask, Gmask, Bmask, Amask));

  SDL_SetSurfaceAlphaMod( surface_.get(), 0xFF );
  SDL_SetSurfaceBlendMode( surface_.get(), SDL_BLENDMODE_NONE );

  SDL_BlitSurface( surface_.get(), NULL, tempSurface.get(), NULL );

  glGenTextures( 1, &textureId_ );
  glBindTexture( GL_TEXTURE_2D, textureId_ );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                width, height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, tempSurface->pixels );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


GLuint Surface::glBind()
{
  assert(surface_ && "Surface not set!");

  if (textureId_ == 0)
    prepareForGl();

  glBindTexture( GL_TEXTURE_2D, textureId_ );
  return textureId_;
}

GLuint Surface::getTextureId() const
{
  return textureId_;
}

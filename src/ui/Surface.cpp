#include <ui/Surface.h>

#include <cassert>

#include <math/MathUtil.h>
#include <ui/SDL_image.h>
#include <util/Asset.h>


Surface::Surface()
  : log_("Surface"),
    surface_(nullptr),
    filename_(""),
    textureId_(0)
{
}


Surface::~Surface()
{
  if (textureId_)
    glDeleteTextures(1, &textureId_);
}


void Surface::loadImage(const std::string& filename)
{
  const std::string path {Asset::path(Asset::IMAGE, filename)};
  log_.d() << "Loading image: " << path << Log::end;

  surface_.reset(IMG_Load(path.c_str()));
  if (!surface_) {
    log_.e() << "Failed to load image " << path << Log::end;
    log_.e() << SDL_GetError() << Log::end;
  }
  filename_ = filename;
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


void Surface::prepareForGl()
{
  assert(surface_ && "Surface not set!");
  assert(textureId_ == 0 && "Already bound to OpenGL!");

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

  if (glGetError() != 0)
    log_.e("Failed when preparing texture");

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

#include "Bitmap.hpp"
#include <stdexcept>

//uses stb_image to try load files
#define STBI_FAILURE_USERMSG
#include "stb_image.c"

inline unsigned char AverageRGB(unsigned char rgb[3]) 
{
    return (unsigned char)(((double)rgb[0] + (double)rgb[1] + (double)rgb[2]) / 3.0);
}

static void Grayscale2GrayscaleAlpha(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
    dest[1] = 255;
}

static void Grayscale2RGB(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
}

static void Grayscale2RGBA(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
    dest[3] = 255;
}

static void GrayscaleAlpha2Grayscale(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
}

static void GrayscaleAlpha2RGB(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
}

static void GrayscaleAlpha2RGBA(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
    dest[3] = src[1];
}

static void RGB2Grayscale(unsigned char* src, unsigned char* dest)
{
    dest[0] = AverageRGB(src);
}

static void RGB2GrayscaleAlpha(unsigned char* src, unsigned char* dest)
{
    dest[0] = AverageRGB(src);
    dest[1] = 255;
}

static void RGB2RGBA(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = 255;
}

static void RGBA2Grayscale(unsigned char* src, unsigned char* dest)
{
    dest[0] = AverageRGB(src);
}

static void RGBA2GrayscaleAlpha(unsigned char* src, unsigned char* dest)
{
    dest[0] = AverageRGB(src);
    dest[1] = src[3];
}

static void RGBA2RGB(unsigned char* src, unsigned char* dest)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

typedef void(*FormatConverterFunc)(unsigned char*, unsigned char*);

static FormatConverterFunc ConverterFuncForFormats(Bitmap::Format srcFormat, Bitmap::Format destFormat)
{
    if(srcFormat == destFormat) {
        throw std::runtime_error("Just use memcpy if pixel formats are the same");
	}
    
    switch(srcFormat){
            
        case Bitmap::Format_Grayscale:
            switch(destFormat){
                case Bitmap::Format_GrayscaleAlpha: return Grayscale2GrayscaleAlpha;
                case Bitmap::Format_RGB:            return Grayscale2RGB;
                case Bitmap::Format_RGBA:           return Grayscale2RGBA;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;
            
        case Bitmap::Format_GrayscaleAlpha:
            switch(destFormat){
                case Bitmap::Format_Grayscale: return GrayscaleAlpha2Grayscale;
                case Bitmap::Format_RGB:       return GrayscaleAlpha2RGB;
                case Bitmap::Format_RGBA:      return GrayscaleAlpha2RGBA;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;
            
        case Bitmap::Format_RGB:
            switch(destFormat){
                case Bitmap::Format_Grayscale:      return RGB2Grayscale;
                case Bitmap::Format_GrayscaleAlpha: return RGB2GrayscaleAlpha;
                case Bitmap::Format_RGBA:           return RGB2RGBA;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;
            
        case Bitmap::Format_RGBA:
            switch(destFormat){
                case Bitmap::Format_Grayscale:      return RGBA2Grayscale;
                case Bitmap::Format_GrayscaleAlpha: return RGBA2GrayscaleAlpha;
                case Bitmap::Format_RGB:            return RGBA2RGB;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;
            
        default:
            throw std::runtime_error("Unhandled bitmap format");
    }
}

                    
inline unsigned GetPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Bitmap::Format format) 
{
    return (row*width + col)*format;
}

inline bool RectsOverlap(unsigned srcCol, unsigned srcRow, unsigned destCol, unsigned destRow, unsigned width, unsigned height)
{
    unsigned colDiff = srcCol > destCol ? srcCol - destCol : destCol - srcCol;
    if(colDiff < width)
        return true;
    
    unsigned rowDiff = srcRow > destRow ? srcRow - destRow : destRow - srcRow;
    if(rowDiff < height)
        return true;
    
    return false;
}

Bitmap::Bitmap(unsigned width, unsigned height, Format format, const unsigned char* pixels) : m_pPixels(NULL)
{
    _set(width, height, format, pixels);
}

Bitmap::~Bitmap() 
{
    if(m_pPixels) free(m_pPixels);
}

Bitmap Bitmap::bitmapFromFile(std::string filePath) 
{    
    int width, height, channels;
    unsigned char* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if(!pixels) {
		throw std::runtime_error(stbi_failure_reason());
	}
    
    Bitmap bmp(width, height, (Format)channels, pixels);
    stbi_image_free(pixels);
    return bmp;
}

Bitmap::Bitmap(const Bitmap& other) : m_pPixels(NULL)
{
    _set(other.m_width, other.m_height, other.m_format, other.m_pPixels);
}

Bitmap& Bitmap::operator = (const Bitmap& other) 
{
    _set(other.m_width, other.m_height, other.m_format, other.m_pPixels);
    return *this;
}

unsigned int Bitmap::width() const
{
    return m_width;
}

unsigned int Bitmap::height() const 
{
    return m_height;
}

Bitmap::Format Bitmap::format() const 
{
    return m_format;
}

unsigned char* Bitmap::pixelBuffer() const 
{
    return m_pPixels;
}

unsigned char* Bitmap::getPixel(unsigned int column, unsigned int row) const 
{
    if(column >= m_width || row >= m_height) {
        throw std::runtime_error("Pixel coordinate out of bounds");
	}
    
    return m_pPixels + GetPixelOffset(column, row, m_width, m_height, m_format);
}

void Bitmap::setPixel(unsigned int column, unsigned int row, const unsigned char* pixel) 
{
    unsigned char* myPixel = getPixel(column, row);
    memcpy(myPixel, pixel, m_format);
}

void Bitmap::flipVertically() 
{
    unsigned long rowSize = m_format*m_width;
    unsigned char* rowBuffer = new unsigned char[rowSize];
    unsigned halfRows = m_height / 2;
    
    for(unsigned rowIdx = 0; rowIdx < halfRows; ++rowIdx){
        unsigned char* row = m_pPixels + GetPixelOffset(0, rowIdx, m_width, m_height, m_format);
        unsigned char* oppositeRow = m_pPixels + GetPixelOffset(0, m_height - rowIdx - 1, m_width, m_height, m_format);
        
        memcpy(rowBuffer, row, rowSize);
        memcpy(row, oppositeRow, rowSize);
        memcpy(oppositeRow, rowBuffer, rowSize);
    }
    
    delete rowBuffer;
}

void Bitmap::rotate90CounterClockwise() 
{
    unsigned char* newPixels = (unsigned char*) malloc(m_format*m_width*m_height);
    
    for(unsigned row = 0; row < m_height; ++row){
        for(unsigned col = 0; col < m_width; ++col){
            unsigned srcOffset = GetPixelOffset(col, row, m_width, m_height, m_format);
            unsigned destOffset = GetPixelOffset(row, m_width - col - 1, m_height, m_width, m_format);
            memcpy(newPixels + destOffset, m_pPixels + srcOffset, m_format); //copy one pixel
        }
    }
    
    free(m_pPixels);
    m_pPixels = newPixels;
    
    unsigned swapTmp = m_height;
    m_height = m_width;
    m_width = swapTmp;
}

void Bitmap::copyRectFromBitmap(const Bitmap& src, unsigned srcCol, unsigned srcRow, unsigned destCol, unsigned destRow, unsigned width, unsigned height)
{
    if (srcCol == 0 && srcRow == 0 && width == 0 && height == 0){
        width = src.width();
        height = src.height();
    }
    
    if(width == 0 || height == 0) {
        throw std::runtime_error("Can't copy zero height/width rectangle");
	}
    
    if(srcCol + width >= src.width() || srcRow + height >= src.height()) {
        throw std::runtime_error("Rectangle doesn't fit within source bitmap");
	}

    if(destCol + width >= m_width || destRow + height >= m_height) {
        throw std::runtime_error("Rectangle doesn't fit within destination bitmap");
	}
    
    if(m_pPixels == src.m_pPixels && RectsOverlap(srcCol, srcRow, destCol, destRow, width, height)) {
        throw std::runtime_error("Source and destination are the same bitmap, and rects overlap. Not allowed!");
	}
    
    FormatConverterFunc converter = NULL;
    if(m_format != src.m_format) {
        converter = ConverterFuncForFormats(m_format, src.m_format);
	}
    
    for(unsigned row = 0; row < height; ++row){
        for(unsigned col = 0; col < width; ++col){
            unsigned char* srcPixel = src.m_pPixels + GetPixelOffset(srcCol + col, srcRow + row, src.m_width, src.m_height, src.m_format);
            unsigned char* destPixel = m_pPixels + GetPixelOffset(destCol + col, destRow + row, m_width, m_height, m_format);
            
            if(converter) {
                converter(srcPixel, destPixel);
            } else {
                memcpy(destPixel, srcPixel, m_format);
            }
        }
    }
}

void Bitmap::_set(unsigned width, unsigned height, Format format, const unsigned char* pixels)
{
    if(width == 0) {
		throw std::runtime_error("Zero width bitmap");
	}
	if(height == 0) {
		throw std::runtime_error("Zero height bitmap");
	}
	if(format <= 0 || format > 4) {
		throw std::runtime_error("Invalid bitmap format");
	}

    m_width = width;
    m_height = height;
    m_format = format;
    
    size_t newSize = m_width * m_height * m_format;
    if(m_pPixels){
        m_pPixels = (unsigned char*)realloc(m_pPixels, newSize);
    } else {
        m_pPixels = (unsigned char*)malloc(newSize);
    }
    
    if(pixels)
        memcpy(m_pPixels, pixels, newSize);
}












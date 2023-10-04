/*MIT License

Copyright (c) 2023 Shahryar Ahmad 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <vector>
#include <stdexcept>
#include <exception>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

class u8string
{
private:
  vector<uint8_t> bytes;
  size_t len = 0;
public:
   static int npos; // unicode's max codepoint is (2^21 -1) or 0x1fffff
   //so it will never be INT_MAX
  u8string(const char* k) //initializing from array of bytes or utf8 as cstring
  {
    int l = strlen(k);
    int len = 0; //length in terms of unicode characters
    for(int i=0;i<l;i++)
    {
      uint8_t byte = k[i];
      if(!(byte & 0x80)) //single byte codepoint
      {
        bytes.push_back(byte);
        len++;
      }
      else if((byte & 0xe0) == 0xc0) // 2 byte codepoint
      {
        if(i+1 >= l || (k[i+1] & 0xc0) != 0x80) //not enough bytes
        {
          bytes.clear();
          throw logic_error("invalid byte stream!");
        }
        bytes.push_back(byte);
        bytes.push_back(k[i+1]);
        i+=1;
        len++;
        continue;
      }
      else if((byte & 0xf0) == 0xe0) // 3 byte codepoint
      {
        if(i+2 >= l || (k[i+1] & 0xc0) != 0x80 || (k[i+2] & 0xc0) != 0x80) //not enough bytes
        {
          bytes.clear();
          throw logic_error("invalid byte stream!");
        }
        bytes.push_back(byte);
        bytes.push_back(k[i+1]);
        bytes.push_back(k[i+2]);
        i+=2;
        len++;
        continue;
      }
      else if((byte & 0xf8) == 0xf0) // 4 byte codepoint
      {
        if(i+3 >= l || (k[i+1] & 0xc0) != 0x80 || (k[i+2] & 0xc0) != 0x80 || (k[i+3] & 0xc0) != 0x80) //not enough bytes
        {
          bytes.clear();
          throw logic_error("invalid byte stream!");
        }
        bytes.push_back(byte);
        bytes.push_back(k[i+1]);
        bytes.push_back(k[i+2]);
        bytes.push_back(k[i+3]);
        len++;
        i+=3;
        continue;
      }
      else
      {
        //invalid byte stream
        bytes.clear();
        throw logic_error("invalid byte stream!");
      }  
    }
    this->len = len;
  }
  u8string(vector<uint8_t> k)
  {
    size_t l = k.size();
    size_t len = 0; //length in terms of unicode characters
    for(size_t i=0;i<l;i++)
    {
      uint8_t byte = k[i];
      if(!(byte & 0x80)) //single byte codepoint
      {
        bytes.push_back(byte);
        len++;
      }
      else if((byte & 0xe0) == 0xc0) // 2 byte codepoint
      {
        if(i+1 >= k.size() || (k[i+1] & 0xc0) != 0x80) //not enough bytes
        {
          bytes.clear();
          throw logic_error("invalid byte stream!");
        }
        bytes.push_back(byte);
        bytes.push_back(k[i+1]);
        i+=1;
        len++;
        continue;
      }
      else if((byte & 0xf0) == 0xe0) // 3 byte codepoint
      {
        if(i+2 >= k.size() || (k[i+1] & 0xc0) != 0x80 || (k[i+2] & 0xc0) != 0x80) //not enough bytes
        {
          bytes.clear();
          throw logic_error("invalid byte stream!");
        }
        bytes.push_back(byte);
        bytes.push_back(k[i+1]);
        bytes.push_back(k[i+2]);
        i+=2;
        len++;
        continue;
      }
      else if((byte & 0xf8) == 0xf0) // 4 byte codepoint
      {
        if(i+3 >= k.size() || (k[i+1] & 0xc0) != 0x80 || (k[i+2] & 0xc0) != 0x80 || (k[i+3] & 0xc0) != 0x80) //not enough bytes
        {
          bytes.clear();
          throw logic_error("invalid byte stream!");
        }
        bytes.push_back(byte);
        bytes.push_back(k[i+1]);
        bytes.push_back(k[i+2]);
        bytes.push_back(k[i+3]);
        len++;
        i+=3;
        continue;
      }
      else
      {
        //invalid byte stream
        bytes.clear();
        throw logic_error("invalid byte stream!");
      }
      	//1111 0XXX
        //0xf8 = 0xf0  
    }
    this->len = len;
  }
  u8string(const u8string& obj)
  {
    len = obj.len;
    bytes = obj.bytes;
  }
  u8string& operator=(u8string rhs)
  {
    if(&rhs == this)
      return *this;
    bytes = rhs.bytes;
    len = rhs.len;
    return *this;
  }
  bool operator==(u8string rhs) const
  {
     return bytes == rhs.bytes;
  }
  bool operator!=(u8string rhs) const
  {
     return bytes != rhs.bytes;
  }
  int operator[](size_t idx) const // returns codepoint as an integer of a char at specific index
  {
    if(idx>=len)
      throw out_of_range("Index out of range!");
    size_t len = 0; //length in terms of unicode characters
    const vector<uint8_t>& k = bytes;
    size_t l = k.size();
    for(size_t i=0;i<l;i++)
    {
      uint8_t byte = k[i];
      if(!(byte & 0x80)) //single byte codepoint
      {
        len++;
        if(len == idx+1)
          return (int)byte;
      }
      else if((byte & 0xe0) == 0xc0) // 2 byte codepoint
      {
        i+=1;
        len++;
        if(len == idx+1)
        {
          int a = byte;
          int b = k[i];
          return ((a & 0x1f)<<6) | (b & 0x3f); 
        }
        continue;
      }
      else if((byte & 0xf0) == 0xe0) // 3 byte codepoint
      {
        i+=2;
        len++;
        if(len == idx+1)
        {
          int a = byte;
          int b = k[i-1];
          int c = k[i];
          return ((((a & 0xf)<<6) | (b & 0x3f))<<6) | (c & 0x3f);
        }
        continue;
      }
      else if((byte & 0xf8) == 0xf0) // 4 byte codepoint
      {
        if(len == idx)
        {
          int a = byte;
          int b = k[i+1];
          int c = k[i+2];
          int d = k[i+3];
          int x = ((((((a & 0x07)<<6) | (b & 0x3f))<<6) | (c&0x3f))<<6) | (d & 0x3f);
          return x;
        }
        len++;
        i+=3;
        continue;
      }
    }
    throw logic_error("Unexpected error");
    return 0;//to avoid warning
  }
  int find(int cp) const // find index of a codepoint
  {
    size_t len = 0; //length in terms of unicode characters
    const vector<uint8_t>& k = bytes;
    int l = k.size();
    for(int i=0;i<l;i++)
    {
      uint8_t byte = k[i];
      if(!(byte & 0x80)) //single byte codepoint
      {
        if(byte == cp)
          return len;
        len++;
      }
      else if((byte & 0xe0) == 0xc0) // 2 byte codepoint
      {
        i+=1;
        len++;
        int a = byte;
        int b = k[i];
        int g = ((a & 0x1f)<<6) | (b & 0x3f); 
        if(g == cp)
          return len-1;
        continue;
      }
      else if((byte & 0xf0) == 0xe0) // 3 byte codepoint
      {
        i+=2;
        len++;
        int a = byte;
        int b = k[i-1];
        int c = k[i];
        int g = ((((a & 0xf)<<6) | (b & 0x3f))<<6) | (c & 0x3f);
        if(g == cp)
          return len-1;
        continue;
      }
      else if((byte & 0xf8) == 0xf0) // 4 byte codepoint
      {
        int a = byte;
        int b = k[i+1];
        int c = k[i+2];
        int d = k[i+3];
        int x = ((((((a & 0x07)<<6) | (b & 0x3f))<<6) | (c&0x3f))<<6) | (d & 0x3f);
        if(x == cp)
          return len;       
        len++;
        i+=3;
        continue;
      }
    }
    return npos;
  }
  int find(const u8string& x) const
  {
    if(x.bytes.size()==0 || bytes.size()==0 || x.bytes.size()>bytes.size())
      return npos;
    size_t len = 0; //length in terms of unicode characters
    const vector<uint8_t>& k = bytes;
    int l = k.size();
    for(int i=0;i<l;i++)
    {
      uint8_t byte = k[i];
      if(i + x.bytes.size()-1 >= k.size()) // no way we will find a match
        return npos;
      if(byte == x.bytes[0])
      {
        bool matched = true;
        for(size_t m=1;m<x.bytes.size();m+=1)
        {
          if(k[i+m] != x.bytes[m])
          {
            matched = false;
            break;
          }
        }
        if(matched)
          return len;
      }
      //advance using utf8 rules
      if(!(byte & 0x80)) //single byte codepoint
      {
        len++;
      }
      else if((byte & 0xe0) == 0xc0) // 2 byte codepoint
      {
        i+=1;
        len++;
        continue;
      }
      else if((byte & 0xf0) == 0xe0) // 3 byte codepoint
      {
        i+=2;
        len++;
        continue;
      }
      else if((byte & 0xf8) == 0xf0) // 4 byte codepoint
      {     
        len++;
        i+=3;
        continue;
      }
    }
    return npos;
  }
  bool replace(const u8string& x,const u8string& y)
  {
    if(x.bytes.size()==0 || bytes.size()==0 || x.bytes.size()>bytes.size())
      return false;
    size_t len = 0; //length in terms of unicode characters
    const vector<uint8_t>& k = bytes;
    int l = k.size();
    for(int i=0;i<l;i++)
    {
      uint8_t byte = k[i];
      if(i + x.bytes.size()-1 >= k.size()) // no way we will find a match
        return false;
      if(byte == x.bytes[0])
      {
        bool matched = true;
        for(size_t m=1;m<x.bytes.size();m+=1)
        {
          if(k[i+m] != x.bytes[m])
          {
            matched = false;
            break;
          }
        }
        if(matched)
        {
          bytes.erase(bytes.begin()+i,bytes.begin()+i+x.bytes.size());
          bytes.insert(bytes.begin()+i,y.bytes.begin(),y.bytes.end());
          this->len = (this->len - x.len) + y.len;
          return true;
        }
      }
      //advance using utf8 rules
      if(!(byte & 0x80)) //single byte codepoint
      {
        len++;
      }
      else if((byte & 0xe0) == 0xc0) // 2 byte codepoint
      {
        i+=1;
        len++;
        continue;
      }
      else if((byte & 0xf0) == 0xe0) // 3 byte codepoint
      {
        i+=2;
        len++;
        continue;
      }
      else if((byte & 0xf8) == 0xf0) // 4 byte codepoint
      {     
        len++;
        i+=3;
        continue;
      }
    }
    return false;
  }
  size_t length() const
  {
    return len;
  }
  void showBytes()
  {
    for(auto e: bytes)
    {
      printf("%x ", e);
    }
    puts("");
  }
  void write(FILE* fp = stdout) const
  {
    if(bytes.size()>0)
      fwrite(&bytes[0],sizeof(uint8_t),bytes.size(),fp);
  }
};
int u8string::npos = INT_MAX;
int main()
{
  #ifdef _WIN32
    SetConsoleOutputCP( 65001 );
  #endif
  //Use utf-8 encoding when saving this file

  //Initialize from vector of bytes
  u8string str({0xe2,0x88,0x82,0x20,0xe2,0x88,0x86,0x20,0xe2,0x88,0x8f,0x20,0xe2,0x88,0x91,'\n'});
  printf("length = %ld\n",str.length());
  str.write();
  printf("\nindividual codepoints in hex: \n");
  for(size_t i=0;i<str.length();i++)
  {
    printf("U+%x\n",str[i]);
  }
  printf("---------------\n");
  //Or initialize from cstring
  u8string another(u8"∂ ∆ ∏ ∑\n");
  another.write();
  printf("%d\n",another.find(u8"∏ ∑"));//search for substring
  printf("%d\n",another.find(0x2211));//search for codepoint
  printf("%d\n",another.find(u8"abc"));//prints INT_MAX
  u8string copy = another;

  copy.replace(u8string(u8"∏ ∑"),u8string(u8"a b"));
  copy.write();
  //
  puts("");
  u8string fruits(vector<uint8_t>{0xf0, 0x9f, 0x8d, 0x8f, 0x20, 0x2d, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x0a, 0xf0, 0x9f, 0x8d, 0x93, 0x20, 0x2d, 0x20, 0x73, 0x74, 0x72, 0x61, 0x77, 0x62, 0x65, 0x72, 0x72, 0x79, 0x0a, 0xf0, 0x9f, 0x8d, 0x91, 0x20, 0x2d, 0x20, 0x70, 0x65, 0x61, 0x63, 0x68, 0x0a, 0xf0, 0x9f, 0x8d, 0x8c, 0x20, 0x2d, 0x20, 0x62, 0x61, 0x6e, 0x61, 0x6e, 0x61, 0x0a, 0xf0, 0x9f, 0x8d, 0x8b, 0x20, 0x2d, 0x20, 0x6c, 0x65, 0x6d, 0x6f, 0x6e, 0x0a, 0xf0, 0x9f, 0x8d, 0x89, 0x20, 0x2d, 0x20, 0x77, 0x61, 0x74, 0x65, 0x72, 0x6d, 0x65, 0x6c, 0x6f, 0x6e, 0x0a, 0xf0, 0x9f, 0x8d, 0x8a, 0x20, 0x2d, 0x20, 0x74, 0x61, 0x6e, 0x67, 0x65, 0x72, 0x69, 0x6e, 0x65, 0x0a, 0xf0, 0x9f, 0x8d, 0x8d, 0x20, 0x2d, 0x20, 0x70, 0x69, 0x6e, 0x65, 0x61, 0x70, 0x70, 0x6c, 0x65,'\n'});
  fruits.write();
  return 0;
}

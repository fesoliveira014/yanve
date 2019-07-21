#pragma once

#include <common.h>

namespace yanve
{
  namespace utils
  {
    inline bool read_file(const char* fileName, std::string& outFile)
    {
      std::ifstream f(fileName);

      bool ret = false;

      if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
          outFile.append(line);
          outFile.append("\n");
        }

        f.close();

        ret = true;
      }

      return ret;
    }

#ifdef _WIN32
    struct matchPathSeparator
    {
      bool operator()(char ch) const
      {
        return ch == '/';
      }
    };

#else
    struct matchPathSeparator
    {
      bool operator()(char ch) const
      {
        return ch == '\\' || ch == '/';
      }
    };
#endif

    struct filename
    {
      std::string name;
      std::string extension;

      filename() {}
      filename(std::string f, std::string ext) : name(f), extension(ext) {}
    };

    static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim)) {
        elems.push_back(item);
      }
      return elems;
    }


    static std::vector<std::string> split(const std::string &s, char delim) {
      std::vector<std::string> elems;
      split(s, delim, elems);
      return elems;
    }

    static filename split_name_and_ext(const std::string& s) {
      std::string name = std::string(
        std::find_if(s.rbegin(), s.rend(),
          matchPathSeparator()).base(),
        s.end());

      std::size_t extensionPosition = name.find_last_of(".");
      return filename(name.substr(0, extensionPosition), name.substr(extensionPosition + 1));

    }

    template <typename T>
    T process_arg(T value) noexcept
    {
      return value;
    }

    template <typename T>
    T const *process_arg(std::basic_string<T> const &value) noexcept
    {
      return value.c_str();
    }

    template <typename... Args>
    std::string stringFormat(const std::string &format, Args const &... args)
    {
      const auto fmt = format.c_str();
      const size_t size = static_cast<std::size_t>(std::snprintf(nullptr, 0, fmt, process_arg(args)...)) + 1;
      auto buf = std::make_unique<char[]>(size);
      std::snprintf(buf.get(), size, fmt, process_arg(args)...);
      auto res = std::string(buf.get(), buf.get() + size - 1);
      return res;
    }
  }
}
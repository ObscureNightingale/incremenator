#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <filesystem>

std::vector<std::string> get_sections(std::string file_name)
{
  std::string content;
  std::fstream file;
  std::vector<std::string> sections;

  file.open(file_name, std::ios::in | std::ios::out);

  if (file.is_open())
    while (!file.eof() && std::getline(file, content))
      if (content.find("[") != std::string::npos)
        sections.push_back(content.replace(0, 1, "").replace(content.length() - 1, 1, ""));

  file.close();

  return sections;
}

std::string sanitise(std::string input)
{
  size_t i = 0;
  std::string temp("");
  bool cleared = 0;
  std::string to_add;

  for (; i < input.length(); i++)
  {
    to_add = input[i];

    if (isdigit(input[i]) && (!isdigit(input[i + 1]) && !isdigit(input[i - 1])))
    {
      to_add = std::string(1, '0') + input[i];
      cleared = 1;
    }

    if (cleared == 0 && !isdigit(input[i]) && input[i + 1] == '.')
      to_add += std::string("01");

    temp += to_add;
  }

  return temp;
}

bool ifs(std::string section, std::string value)
{
  if (section.find(value) != std::string::npos && section.find("]") != std::string::npos)
    return true;
  else
    return false;
}

std::string vfa(std::string previous, std::string context, std::string ref, std::string fp)
{
  std::string val("");

  if (ref == "]")
    return std::string("");

  if (context.find("ITEMS") != std::string::npos && context.substr(context.find("{") + 1) != "}")
    val += ", ";

  if (ifs(previous, ref))
    if (context.find(fp) == std::string::npos)
      val += fp;

  if (val == ", ")
    val = std::string("");

  return val;
}

std::vector<std::string> split(std::string values)
{
  std::vector<std::string> temp_items;
  std::vector<std::string> items;
  std::string temp;

  for (const auto &v : values)
  {
    if (v == ' ' || v == ',' || values.back() == v)
    {
      if (v == ' ')
        temp_items.push_back(temp.substr(0, temp.length()));
      else
        temp_items.push_back(temp);
      temp = std::string("");
      continue;
    }

    temp += v;
  }

  for (auto &x : temp_items)
    if (x != " ")
      items.push_back(x);

  return items;
}

void save_organised(std::string latest)
{
  std::string context;
  std::fstream latest_file(latest, std::ios::out | std::ios::in);
  std::fstream organised("/tmp/increment/maps-organised.txt", std::ios::out);

  if (!organised)
  {
    std::cout << "unable to create organised file..\n";
    return;
  }

  organised.close();
  organised.open("/tmp/increment/maps-organised.txt", std::ios::out | std::ios::in);

  while (std::getline(latest_file, context))
  {
    std::string tp(context);
    if (context.find("ITEMS") != std::string::npos)
    {
      std::string items = context.substr(context.find("{") + 1);
      std::vector<std::string> list = split(items);

      sort(list.begin(), list.end());

      tp = "ITEMS={";

      for (auto &x : list)
      {
        if (x.length() < 1)
          continue;

        std::string have(x);
        if (list.back() != x)
          have += ", ";

        tp += have;
      }

      tp += "}";
    }
    organised << tp << std::endl;
  }

  latest_file.close();
  organised.close();

  remove("/tmp/increment/maps.txt");
  rename("/tmp/increment/maps-organised.txt", "/tmp/increment/maps.txt");
}

void save(std::string first, std::string second)
{

  if (!first.length())
    first = "]";

  if (!second.length())
    second = "]";

  std::string content;
  std::fstream file;
  std::string previous;
  std::string to_push_F = sanitise(first);
  std::string to_push_S = sanitise(second);
  std::fstream new_file("/tmp/increment/maps-new.txt", std::ios::out);

  if (!new_file)
  {
    std::cout << "unable to create file.."
              << "\n";
    return;
  }

  new_file.close();
  new_file.open("/tmp/increment/maps-new.txt", std::ios::in | std::ios::out);

  for (const auto _ : get_sections("/tmp/increment/maps.txt"))
  {
    file.open("/tmp/increment/maps.txt", std::ios::in | std::ios::out);

    if (file.is_open())
      while (!file.eof() && getline(file, content))
      {
        if (!content.length())
        {
          previous = " ";
          continue;
        }

        std::string cpy = content;

        std::remove_if(first.begin(), first.end(), [](unsigned char c) { return std::isdigit(c); });
        std::remove_if(second.begin(), second.end(), [](unsigned char c) { return std::isdigit(c); });

        std::string f = first.substr(0, first.find("."));
        std::string _s = second.substr(0, second.find("."));

        std::string test_f = vfa(previous, content, f, to_push_F);
        std::string test_s = vfa(previous, content, _s, to_push_S);

        if (content.find("ITEMS") != std::string::npos)
        {
          content.replace(
              content.length() - 1, 1,
              std::string("")
                  .append(test_f)
                  .append((test_f.length() && test_s.length() && f == _s && test_s.find(", ") == std::string::npos ? std::string(", ") : "") + test_s)
                  .append(
                      previous.find("]") != std::string::npos
                          ? "}"
                          : "]"));
        }

        previous = cpy;

        new_file << content << std::endl;

        if (content.find("ITEMS") != std::string::npos)
          new_file << std::endl;
      }

    new_file.close();
  }

  remove("/tmp/increment/maps.txt");
  rename("/tmp/increment/maps-new.txt", "/tmp/increment/maps.txt");
}

bool has_section(std::string val)
{
  std::fstream dest("/tmp/increment/maps.txt");
  std::string f;
  bool has = 0;

  while (std::getline(dest, f))
  {
    if (f.find("[") != std::string::npos && f.find(val) != std::string::npos)
    {
      has = 1;
      break;
    }
  }

  return has;
}

std::string section_name(std::string name)
{
  std::remove_if(name.begin(), name.end(), [](unsigned char c) { return isdigit(c); });

  return name.substr(0, name.find("."));
}

void cfe(int n, char *vs[])
{
  std::fstream dstr("/tmp/increment/maps.txt", std::ios::out);
  std::string ctx;
  std::fstream nw("/tmp/increment/maps-temp.txt", std::ios::out);
  std::vector<std::string> uniques;

  if (!nw)
  {
    std::cout << "unable to create certified file.."
              << "\n";
    return;
  }

  nw.close();
  nw.open("/tmp/increment/maps-temp.txt", std::ios::out | std::ios::in);

  for (int i = 1; i < n; i++)
  {
    std::string v = section_name(vs[i]);

    // std::cout << v << "\n";

    if (!std::count(uniques.begin(), uniques.end(), v))
      uniques.push_back(v);
  }

  for (auto &v : uniques)
  {
    if (dstr.peek() == std::ifstream::traits_type::eof())
    {
      nw << "[" << v.substr(0, v.find(".")) << "]"
         << "\nITEMS={}\n\n";
      continue;
    }
    else
      while (std::getline(dstr, ctx))
      {
        if (!has_section(v))
        {
          nw << "[" << v.substr(0, v.find(".")) << "]"
             << "\nITEMS={}\n\n";
        }
      }
  }

  nw.close();

  remove("/tmp/increment/maps.txt");
  rename("/tmp/increment/maps-temp.txt", "/tmp/increment/maps.txt");
}

bool certify_dir()
{
  if (std::filesystem::is_directory("/tmp/increment") == true)
    return 1;

  auto dir = std::filesystem::create_directory("/tmp/increment/");

  if (!dir)
    return 0;

  return 1;
}

int main(int argc, char *argv[])
{

  if (!certify_dir())
  {
    std::cout << "INTERNAL ERROR: unable to create directory.."
              << "\n";
    return 0;
  }

  cfe(argc, argv);

  int i = 1;

  while (i < argc)
  {
    if (i + 1 == argc)
      save(argv[i], "");
    else
      save(argv[i], argv[i + 1]);

    i += 2;
  }

  save_organised("/tmp/increment/maps.txt");
}
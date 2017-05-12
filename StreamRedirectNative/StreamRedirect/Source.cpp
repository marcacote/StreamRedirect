#include <json/value.h>
#include <json/reader.h>

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

int main(int argc, char* argv[]) {
  unsigned int length = 0;
  for (int i = 0; i < 4; ++i) {
    unsigned int read_char = getchar();
    length = length | (read_char << i*8);
  }

  //std::string json_msg = "{\"text\":\"nl_Kripp\"}";
  std::string json_msg = "";
  for (unsigned int i = 0; i < length; ++i) {
    json_msg += getchar();
  }

  std::ofstream file;
  file.open("hello.txt");
  file << "Length of message: " << length << std::endl;
  file << "Json message: " << json_msg << std::endl;

  Json::Reader reader;
  Json::Value root;
  if (!reader.parse(json_msg, root)) {
    file << "Failed to parse json: " << json_msg << std::endl;
  }

  std::string text;
  text = text.append("streamlink twitch.tv/");
  text = text.append(root.get("text","failed").asString());
  text = text.append(" best");
  file << "Cmd line: " << text << std::endl;
  std::wstring wtext = std::wstring(text.begin(), text.end());
  LPWSTR cmd_final = (LPWSTR)wtext.c_str();
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  if(!CreateProcess(
      NULL,              // No module name (use command line)
      cmd_final,         // Command line
      NULL,              // Process attributes
      NULL,              // Thread attributes
      FALSE,             // Handle inheritance
      CREATE_NO_WINDOW,  // Creation flags
      NULL,              // Use parent's environment block
      NULL,              // Use parent's starting directory 
      &si,               // Pointer to STARTUPINFO structure
      &pi )              // Pointer to PROCESS_INFORMATION structure
  ) {
      file << "Failed to create process: " << GetLastError() << std::endl;
      return 0;
  }

  file << "Inside text: " << text << std::endl;

  file.close();

  return 0;
}
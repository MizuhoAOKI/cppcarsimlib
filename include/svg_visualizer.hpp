// Simple header-only library to visualize simulation result with svg animation.
#include <iostream>
#include <fstream>
#include "csv.h"
#include <cmath> 
#include <vector>
#include <string>
#include <vector>
#include <sstream>

#define COLUMNS 3
#define TEMPLATE_PATH "../templates/svg_template.svg"


namespace SVGOutput{

  // Ref: https://www.delftstack.com/ja/howto/cpp/string-replace-cpp/
  std::string Replace(std::string& str, const std::string& sub, const std::string& mod) {
      std::string tmp(str);
      tmp.replace(tmp.find(sub), sub.length(), mod);
      return tmp;
  }

  void OutputSVGfromCSV(std::string csvpath, std::string outputpath, double ff_scale=1.0)
  {
    // define variables
    double time;
    double Xo;
    double Yo;
    int compress_val;
    double sim_time;
    double min_x = 0.0;
    double max_x = 0.0;
    double min_y = 0.0;
    double max_y = 0.0;
    double viewbox_min_x = 0.0;
    double viewbox_max_x = 0.0;
    double viewbox_min_y = 0.0;
    double viewbox_max_y = 0.0;
    double path_length = 0.0;
    double pre_Xo = 0.0;
    double pre_Yo = 0.0;

    std::vector<std::vector<double>> csvlog(1, std::vector<double>(COLUMNS));
    std::vector<std::vector<double>> keyrec(1, std::vector<double>(2));
    csvlog.erase(csvlog.begin()); // remove the top row for initialization
    keyrec.erase(keyrec.begin()); // remove the top row for initialization
    
    io::CSVReader<COLUMNS> in(csvpath);
    in.read_header(io::ignore_extra_column, "Time", "Xo", "Yo");
    
    // Load csv file
    while(in.read_row(time, Xo, Yo)){
        std::vector<double> thisrow = {time, Xo, Yo};
        csvlog.push_back(thisrow);
        sim_time = time; // get simulation time
        if(Xo<min_x){min_x=Xo;} // update min_x
        if(Yo<min_y){min_y=Yo;} // update min_y
        if(Xo>max_x){max_x=Xo;} // update max_x
        if(Yo>max_y){max_y=Yo;} // update max_y

        // calc path_length and save it with sim_time for Keypoints and Keytimes attributes.
        path_length += sqrt(pow(Xo-pre_Xo,2) + pow(Yo-pre_Yo,2));
        std::vector<double> thiskey = {time, path_length};
        keyrec.push_back(thiskey);

        pre_Xo = Xo;
        pre_Yo = Yo;
    }

      // set viewbox size adding offset
      double margin_x = (max_x - min_x) / 10; // set margin_x to 10% of width
      double margin_y = (max_y - min_y) / 10; // set margin_y to 10% of height
      double viewbox_width  = (max_x + margin_x) - (min_x - margin_x);
      double viewbox_height = (max_y + margin_y) - (min_y - margin_y);
      std::string svg_path_code = "M " + std::to_string(margin_x) + "," + std::to_string(margin_y) + ", L";
      int csv_row_num = csvlog.size();
      for(int i=0; i<csv_row_num; i++){
          svg_path_code += " " + std::to_string(csvlog.at(i).at(1) + margin_x) + "," + std::to_string(csvlog.at(i).at(2) + margin_y) + " ";
      }

      std::string viewbox_code = std::to_string(min_x) + "," + std::to_string(min_y) + "," + std::to_string(viewbox_width) + "," + std::to_string(viewbox_height);
      std::string keypoints_code = "";
      std::string keytimes_code = "";

      for(int i=0; i<csv_row_num; i++){
          keytimes_code  += std::to_string( keyrec.at(i).at(0) / sim_time);
          keypoints_code += std::to_string( keyrec.at(i).at(1) / path_length);
          if(i!=csv_row_num-1){
              keytimes_code  += "; ";
              keypoints_code += "; ";
          }
      }

      // open svg files
      std::string buf;
      std::ifstream ifs(TEMPLATE_PATH);
      std::ofstream ofs(outputpath);

      // copy ifs to buf
      buf = std::string(std::istreambuf_iterator<char>(ifs),
                        std::istreambuf_iterator<char>());

      if (!ifs) {
          std::cerr << "Error loading the svg_template" << std::endl;
          std::exit(1);
      }

      // replace svg contexts
      buf = Replace(buf, "$TRAJECTORY$", svg_path_code);
      buf = Replace(buf, "$VIEWBOX$"   , viewbox_code); // 0 0 1412 937
      buf = Replace(buf, "$WIDTH$"     , std::to_string(viewbox_width));  
      buf = Replace(buf, "$HEIGHT$"    , std::to_string(viewbox_height));  
      buf = Replace(buf, "$SIMTIME$"   , std::to_string(sim_time/ff_scale)); 
      buf = Replace(buf, "$KEYTIMES$"  , keytimes_code);
      buf = Replace(buf, "$KEYPOINTS$" , keypoints_code);       

      // output svg file
      ofs << buf << std::endl;
      ofs.close();

      std::cout << csvpath << " saved." <<std::endl;
  }

}
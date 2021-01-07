#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <regex>

std::string getType(std::string value) {
   if(value.find('.')) {
       return "double";
   }
   else if(value.find('"')) {
       return "string";
   }
   else {
       return "int";
   }
};

std::vector<std::string> split(std::string text, char delim) {
    std::string line;
    std::vector<std::string> vec;
    std::stringstream ss(text);
    while(std::getline(ss, line, delim)) {
        vec.push_back(line);
    }
    return vec;
}

std::vector<std::string> filterByRegex(std::string str){
    std::regex cond("\\s*(\")|(([a-zA-Z\u0530-\u058F\uFB00-\uFB17]+)(\\d*)|[+*/-])|([0-9]+).([0-9]+)|([0-9]+)|!=|<=|>=|==|[=]|<|>|%\\s*");
    std::vector<std::string> arr;
    std::sregex_token_iterator iterStart(str.begin(), str.end(), cond, 0);
    std::sregex_token_iterator iterEnd;
    while(iterStart != iterEnd){
        arr.push_back(*iterStart);
        ++iterStart;
    }
    return arr;
}

class MyType{
    public:
        std::string name;
        std::string value;
        std::string type;
};

bool ifOnCondition = true;
bool ifOnConditionEnded = true;
std::vector<std::string> loopCondition;
bool ifLoop = false;
std::vector<std::string> loopBody;

std::vector<MyType> myAllVariables;

MyType findInMyAllVariables(std::string name) {
   for(int i = 0; i < myAllVariables.size(); ++i) {
       if(myAllVariables[i].name == name) {
           return myAllVariables[i];
       } 
       else {
           return MyType();
       }
   };
   return MyType();
}

void changeVariable(std::string var, std::string val){
    for(int i = 0; i < myAllVariables.size(); ++i) {
       if(myAllVariables[i].name == var) {
            myAllVariables[i].value = val;
       }
   };
}

std::string getIfIsSet(std::string var){
    MyType item = findInMyAllVariables(var);
    if (item.name != "")
    {
        return item.value;
    }else{
        return var;
    }
    
}

bool isSet(std::string var){
    MyType item = findInMyAllVariables(var);
    if (item.name != "")
    {
        return true;
    }
    return false;
    
}

std::vector<std::string> doMultiply(std::vector<std::string> v){
    int sum = 0;
    auto it = find(v.begin(), v.end(), "*");
    if (it != v.end())
    {
        sum = std::stoi( getIfIsSet(v[it - v.begin() - 1]) ) * std::stoi( getIfIsSet(v[it - v.begin() + 1]) ) ;
        v[it - v.begin() - 1] = std::to_string(sum);
        v.erase(it);
        v.erase(it);
        if (find(v.begin(), v.end(), "*") != v.end())
        {
            return doMultiply(v);
        }else{
            return v;
        }
    }else{
        return v;
    }
}

std::vector<std::string> doDivision(std::vector<std::string> v){
    int sum = 0;
    auto it = find(v.begin(), v.end(), "/");
    if (it != v.end())
    {
        sum = std::stoi( getIfIsSet(v[it - v.begin() - 1]) ) / std::stoi( getIfIsSet(v[it - v.begin() + 1]) ) ;
        v[it - v.begin() - 1] = std::to_string(sum);
        v.erase(it);
        v.erase(it);
        if (find(v.begin(), v.end(), "/") != v.end())
        {
            return doDivision(v);
        }else{
            return v;
        }
    }else{
        return v;
    }
}

std::vector<std::string> doIncrement(std::vector<std::string> v){
    int sum = 0;
    auto it = find(v.begin(), v.end(), "+");
    if (it != v.end())
    {
        sum = std::stoi( getIfIsSet(v[it - v.begin() - 1]) ) + std::stoi( getIfIsSet(v[it - v.begin() + 1]) ) ;
        v[it - v.begin() - 1] = std::to_string(sum);
        v.erase(it);
        v.erase(it);
        if (find(v.begin(), v.end(), "+") != v.end())
        {
            return doIncrement(v);
        }else{
            return v;
        }
    }else{
        return v;
    }
}

std::vector<std::string> doDecrement(std::vector<std::string> v){
    int sum = 0;
    auto it = find(v.begin(), v.end(), "-");
    if (it != v.end())
    {
        sum = std::stoi( getIfIsSet(v[it - v.begin() - 1]) ) - std::stoi( getIfIsSet(v[it - v.begin() + 1]) ) ;
        v[it - v.begin() - 1] = std::to_string(sum);
        v.erase(it);
        v.erase(it);
        if (find(v.begin(), v.end(), "-") != v.end())
        {
            return doDecrement(v);
        }else{
            return v;
        }
    }else{
        return v;
    }
}

void analyzer(std::vector<std::string> line , std::string strLine){
    if(line[0] == "մկա") {
        MyType temporaryObject;
        temporaryObject.name = line[1];
        temporaryObject.value = line[3];
        temporaryObject.type = getType(temporaryObject.value);
        myAllVariables.push_back(temporaryObject);
    }
    else if(line[0] == "թխե") {
        if (line[1] == " \"")
        {
            std::cout << line[2] << std::endl;
        }else{
            MyType temporaryObject = findInMyAllVariables(line[1]);
            if(temporaryObject.name != "") {
                std::cout << temporaryObject.value << std::endl;
            }
            else {
                std::cout << line[1] << std::endl;
            }
        }
    }
    else {
        if (isSet(line[0]))
        {
            line = doMultiply(line);

            line = doDivision(line);

            line = doDecrement(line);

            line = doIncrement(line);

            changeVariable(line[0], line[2]);
        }else{
             std::cout<< "Syntax error" << std::endl;
        }

    }    

}

void doLoop(std::vector<std::string> v){
    for (int j = 0; j < v.size(); ++j)
    {
        std::vector<std::string> loopToken = filterByRegex(v[j]);
        analyzer(loopToken, "");
    }
}

void changeIfConds(){
    ifOnCondition = true;
    ifOnConditionEnded = true;
}

void readFile(const std::string& fileName) {
    std::string line;
    std::ifstream myFile;
    myFile.open(fileName);
    if (myFile.is_open()) {
        while ( getline(myFile,line) ) {

            std::vector<std::string> parsedToken = split(line, ';');

            if (parsedToken.size() > 1)
            {     
                for (int i = 0; i < parsedToken.size(); i++)
                {
                    std::vector<std::string> childToken = filterByRegex(parsedToken[i]);

                    if (ifLoop && childToken[0] != "ֆռաավարտ")
                    {
                        loopBody.push_back(line);
                        continue;
                    }
                    if (childToken[0] == "ֆռա")
                    {
                        loopCondition = childToken;
                        ifLoop = true;
                        continue;
                    }
                    if (childToken[0] == "ֆռաավարտ")
                    {
                        ifLoop = false;

                        std::string markLoop = loopCondition.at(loopCondition.size() - 2);

                        if (markLoop == "<")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) < std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == "<=")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) <= std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == ">" )
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) > std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == ">=")
                        { 
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) >= std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == "==")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) == std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == "!=")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) != std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }                           
                        }

                        continue;
                    }
                    

                    if (childToken[0] == "հակառակ")
                    {
                        if (ifOnCondition)
                        {
                            ifOnConditionEnded = false;
                            ifOnCondition = false;
                            continue;
                        }
                        else{
                            changeIfConds();
                            continue;
                        }
                    }

                    if (childToken[0] == "եթեավարտ")
                    {
                        changeIfConds();
                        continue;
                    }
                    

                    if (!ifOnCondition && !ifOnConditionEnded)
                    {
                        continue;
                    }

                    if (childToken[0] == "եթե")
                    {
                        std::string mark = childToken.at(childToken.size() - 2);

                        if (mark == "<" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) < std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == "<=" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) <= std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == ">" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) > std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == ">=" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) >= std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        { 
                            changeIfConds();
                            continue;
                        }
                        else if (mark == "==" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) == std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == "!=" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) != std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;                            
                        }

                        ifOnCondition = false;
                        continue;
                    }

                    if (ifOnCondition){
                        analyzer(childToken, parsedToken[i]);
                    }
                }
            }
            else{
                if (line != "")
                {
                    std::vector<std::string> childToken = filterByRegex(line);

                    if (ifLoop && childToken[0] != "ֆռաավարտ")
                    {
                        loopBody.push_back(line);
                        continue;
                    }
                    if (childToken[0] == "ֆռա")
                    {
                        loopCondition = childToken;
                        ifLoop = true;
                        continue;
                    }
                    if (childToken[0] == "ֆռաավարտ")
                    {
                        ifLoop = false;

                        std::string markLoop = loopCondition.at(loopCondition.size() - 2);

                        if (markLoop == "<")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) < std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == "<=")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) <= std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == ">" )
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) > std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == ">=")
                        { 
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) >= std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == "==")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) == std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }
                        }
                        else if (markLoop == "!=")
                        {
                            while (std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 3)) ) != std::stoi( getIfIsSet(loopCondition.at(loopCondition.size() - 1))))
                            {
                                doLoop(loopBody);
                            }                           
                        }

                        continue;
                    }
                    

                    if (childToken[0] == "հակառակ")
                    {
                        if (ifOnCondition)
                        {
                            ifOnConditionEnded = false;
                            ifOnCondition = false;
                            continue;
                        }
                        else{
                            changeIfConds();
                            continue;
                        }
                    }

                    if (childToken[0] == "եթեավարտ")
                    {
                        changeIfConds();
                        continue;
                    }
                    

                    if (!ifOnCondition && !ifOnConditionEnded)
                    {
                        continue;
                    }

                    if (childToken[0] == "եթե")
                    {
                        std::string mark = childToken.at(childToken.size() - 2);

                        if (mark == "<" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) < std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == "<=" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) <= std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == ">" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) > std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == ">=" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) >= std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        { 
                            changeIfConds();
                            continue;
                        }
                        else if (mark == "==" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) == std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;
                        }
                        else if (mark == "!=" && std::stoi( getIfIsSet(childToken.at(childToken.size() - 3)) ) != std::stoi( getIfIsSet(childToken.at(childToken.size() - 1))))
                        {
                            changeIfConds();
                            continue;                            
                        }

                        ifOnCondition = false;
                        continue;
                    }

                    if (ifOnCondition){
                        analyzer(childToken, line);
                    }
                }
            }
        }
        myFile.close();
    }
    else {
        std::cout << "Unable to open file";
    } 
   
}

int main(int argc, char** argv) {
    readFile(argv[1]);
    myAllVariables.clear();
}
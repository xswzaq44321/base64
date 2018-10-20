#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "base64.h"
#include "myerr.h"
using namespace std;

enum Mode{
	none, encode, decode
};

int main(int argc, char *argv[]){
	Mode mode = none;
	string buffer;
	string ifilename, ofilename;
	try{
		int pos = 1;
		while(pos < argc){
			if(argv[pos][0] == '-'){
				switch(argv[pos][1]){
					case 'f':
						if(pos + 1 >= argc || argv[pos + 1][0] == '-')
							throw myerr::wrong_argument("missing argument after [-f]");
						ifilename.assign(argv[pos + 1]);
						pos += 2;
						break;
					case 'o':
						if(pos + 1 >= argc || argv[pos + 1][0] == '-')
							throw myerr::wrong_argument("missing argument after [-o]");
						ofilename.assign(argv[pos + 1]);
						pos += 2;
						break;
					case 'e':
						if(mode != none)
							throw myerr::wrong_argument("you can only choose encode or decode");
						mode = encode;
						++pos;
						break;
					case 'd':
						if(mode != none)
							throw myerr::wrong_argument("you can only choose encode or decode");
						mode = decode;
						++pos;
						break;
					default:
						throw myerr::argument_syntax_error();
						break;
				}
			}else{
				throw myerr::argument_syntax_error();
			}
		}
		if(mode == none)
			throw myerr::argument_syntax_error("please select encode or decode");
	}catch(const myerr::argument_syntax_error err){
		cerr << "Error! " << err.what << endl;
		cerr << "Usage: " << __FILE__ << "\b\b\b\b" << " [-f][filename] [-o][filename]\n";
		exit(1);
	}catch(const myerr::wrong_argument err){
		cerr << "Error!\n";
		cerr << err.what;
	}

	if(!ifilename.empty()){
		ifstream is(ifilename, std::ifstream::binary);
		if(is){
			is.seekg(0, is.end);
			int length = is.tellg();
			is.seekg(0, is.beg);
			char *temp = new char[length];

			cout << "Reading " << length << " characters... \n";
			is.read(temp, length);
			buffer.assign(temp);

			if(is)
				cout << "all character read successfully.\n";
			else
				cout << "error: only " << is.gcount() << " could be read.\n";
			is.close();
			delete[] temp;
		}else{
			cerr << "error: could not find file " << ifilename << ".\n";
			exit(1);
		}
	}else{
		getline(cin, buffer);
	}
	for(int i = buffer.length() - 2; i < buffer.length(); ++i){
		if(buffer[i] == '\n' || buffer[i] == '\r'){
			buffer.erase(i, 1);
			--i;
		}
	}
	if(mode == decode){
		for(int i = 0; i < buffer.length(); ++i){
			if(buffer[i] == '\n' || buffer[i] == '\r'){
				buffer.erase(i, 1);
				--i;
			}
		}
	}

	string buffer2;
	if(mode == encode){
		buffer2 = base64::encode(buffer);
	}else{
		buffer2 = base64::decode(buffer);
	}
	cout << "{" << buffer2 << "}";

	if(!ofilename.empty()){
		ofstream os(ofilename, std::ofstream::out);

		cout << "\n\nWriting " << buffer2.length() << " characters into \"" << ofilename << "\"";
		os << buffer2 << endl;
		//os.write(buffer2.c_str(), buffer2.length());
		os.close();
	}
}

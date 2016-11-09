#include "FileIO.h"

FileIO::FileIO()
{
}


FileIO::~FileIO()
{
}
/*
void FileWriter::SaveLightShapes(std::string path, std::vector<std::shared_ptr<ltbl::LightShape>>& list)
{
	std::ofstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}
	
	file << "start\n";
	sf::ConvexShape ls;
	//stackoverflow
	for (std::vector<std::shared_ptr<ltbl::LightShape>>::iterator it = list.begin(); it != list.end(); ++it)
	{
		ls = it->get()->_shape;
		file << "id:" << it - list.begin() << std::endl;
		file << ls.getPosition().x << ":" << ls.getPosition().y << std::endl;
		file << ls.getPointCount() << std::endl;
		for (int i = 0; i < ls.getPointCount(); i++)
		{
			file << ls.getPoint(i).x << ":" << ls.getPoint(i).y << std::endl;
		}

	}
	file << "end\n";
	file.close();
}

void FileWriter::LoadLightShapesFromFile(std::string path, std::vector<std::shared_ptr<ltbl::LightShape>>& list, ltbl::LightSystem& ls)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	//unsigned int Index = list.size();
	size_t index = 0,
		pnt_cnt = 0;
	std::string tmp = "";

	do
	{
		std::getline(file, tmp);
		//std::cout << tmp << std::endl;
		//std::cout << "nothing found...\n";
	} while (tmp.compare("start") != 0 && !file.eof());

	if (tmp.compare("start") == 0)
	{
		list.clear();
		//std::cout << "start found!\n";
	}
	else
	{
		std::cout << "something went wrong\n";
		return;
	}

	std::getline(file, tmp, '\n');
	int init = 0;

	do
	{
		//int pos = tmp.find(':');
		size_t pos = tmp.find(':');
		if (tmp.substr(0, pos).compare("id") == 0)
		{
			if(init > 0)
				ls.addShape(list.at(index));

			//index++;
			index = list.size();
			pnt_cnt = 0;
			//std::cout << "*** new light shape ***\nid = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list.push_back(std::make_shared<ltbl::LightShape>());
			std::getline(file, tmp, '\n');
			pos = tmp.find(':');
			//std::cout << "origin: x = " << tmp.substr(0, pos) << "\ty = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list.at(index)->_shape.setPosition( std::stof(tmp.substr(0, pos)), std::stof(tmp.substr(pos + 1, tmp.length() - pos)));
			std::getline(file, tmp, '\n');
			//std::cout << "# of point = " << tmp << std::endl;
			list.at(index)->_shape.setPointCount(std::stoi(tmp));
		}
		else
		{
			//std::cout << "x = " << tmp.substr(0, pos) << "\ty = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list.at(index)->_shape.setPoint(pnt_cnt, sf::Vector2f(std::stof(tmp.substr(0, pos)), std::stof(tmp.substr(pos + 1, tmp.length() - pos))));
			pnt_cnt++;
		}
		std::getline(file, tmp, '\n');
		init = 1;
	} while (tmp.compare("end") != 0 && !file.bad() && !file.eof());

	ls.addShape(list.at(index));

	file.close();
}*/

sf::Vector2f FileIO::LoadLevel(const char* path, std::vector<gameobj>& obstacles, std::vector<sf::Texture*>& textures)
{
	sf::Vector2f size(0, 0);

	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return sf::Vector2f(0, 0);
	}

	do {
		std::string tmp;
		std::getline(file, tmp);

		if (tmp.size() == 0)
			break;

		if (tmp[0] == 't')
		{
			char tex_path[255];
			sscanf_s(tmp.c_str(), "t,%s", tex_path);

			sf::Texture* tmp_tex = new sf::Texture();
			tmp_tex->loadFromFile(tex_path);
			textures.push_back(tmp_tex);

			std::cout << "image path " << textures.size() << " = " << tex_path << std::endl;
		}
		else if (tmp[0] == 'w')
		{
			if (textures.size() > 0)
				exit(1);

			char tex_path[255];
			sscanf_s(tmp.c_str(), "w,%s", tex_path);

			sf::Texture* tmp_tex = new sf::Texture();
			tmp_tex->loadFromFile(tex_path);
			tmp_tex->setRepeated(true);

			textures.push_back(tmp_tex);

			std::cout << "world image path = " << tex_path << std::endl;
		}
		else if (tmp[0] == 's')
		{
			sscanf_s(tmp.c_str(), "s,%f,%f", &size.x, &size.y);

			std::cout << "world size = " << size.x << ":" << size.y << std::endl;
		}
		else
		{
			gameobj tmp_obj;
			sf::Vector2f pos;
			float rot;
			sscanf_s(tmp.c_str(), "%i,%f,%f,%f", &tmp_obj.textureId, &pos.x, &pos.y, &rot);
			tmp_obj.setPosition(pos);
			tmp_obj.setRotation(rot);
			tmp_obj.sprite.setTexture(*textures[tmp_obj.textureId+1]);
			obstacles.push_back(tmp_obj);

			std::cout << "index = " << tmp_obj.textureId << ", position = " << pos.x << ":" << pos.y << std::endl;
		}
		
	} while (!file.eof());

	file.close();

	return size;
}

std::string FileIO::LoadText(const char* path)
{
	std::ifstream file(path);
	//file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return "Error";
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	file.close();

	return content;
}

#include "FileIO.h"

FileIO::FileIO()
{
}


FileIO::~FileIO()
{
}

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
			tmp_obj.sprite.setOrigin(textures[tmp_obj.textureId + 1]->getSize().x / 2, textures[tmp_obj.textureId + 1]->getSize().y / 2);
			obstacles.push_back(tmp_obj);

			std::cout << "index = " << tmp_obj.textureId << ", position = " << pos.x << ":" << pos.y << std::endl;
		}
		
	} while (!file.eof());

	file.close();

	return size;
}

bool FileIO::LoadPath(const char * path, std::vector<sf::Vector2f>& waypoints)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return false;
	}

	do {
		std::string tmp;
		std::getline(file, tmp);

		if (tmp.size() == 0)
			break;

		float x, y;
		sscanf_s(tmp.c_str(), "%f,%f", &x, &y);

		waypoints.push_back(sf::Vector2f(x, y));

		//std::cout << "world size = " << size.x << ":" << size.y << std::endl;
	} while (!file.eof());

	file.close();

	return true;
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

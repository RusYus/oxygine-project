#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

#include "JsonImporter.hpp"

namespace Service
{

BOOST_AUTO_TEST_SUITE(JsonImporterTests)

struct JsonImporterFixture
{
    void CreateMapFile(const std::string& aMapName, std::string&& aMapText)
    {
        boost::filesystem::remove(aMapName);
        std::ofstream oStream;
        oStream.open(aMapName, std::ios::out);
        oStream << aMapText;
        oStream.close();
    }

    JsonImporter importer;
};

BOOST_FIXTURE_TEST_CASE(ShouldProcessValidMapFile, JsonImporterFixture)
{
    std::string jsonText ="{ \"height\":6,\n"
        "\"layers\":[\n"
             "{\n"
              "\"data\":[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0],\n"
              "\"height\":6,\n"
              "\"name\":\"\u0421\u043b\u043e\u0439 \u0442\u0430\u0439\u043b\u043e\u0432 1\",\n"
              "\"opacity\":1,\n"
              "\"type\":\"tilelayer\",\n"
              "\"visible\":true,\n"
              "\"width\":4,\n"
              "\"x\":0,\n"
              "\"y\":0\n"
             "}],\n"
        "\"nextobjectid\":1,\n"
        "\"orientation\":\"orthogonal\",\n"
        "\"renderorder\":\"left-up\",\n"
        "\"tileheight\":25,\n"
        "\"tilesets\":[\n"
        "{\n"
              "\"firstgid\":1,\n"
              "\"source\":\"untitled.tsx\"\n"
             "}],\n"
        "\"tilewidth\":25,\n"
        "\"type\":\"map\",\n"
        "\"version\":\"2017.05.16\",\n"
        "\"width\":4\n"
        "}\n";

    const std::string mapName = "valid_map.json";

    CreateMapFile(mapName, std::move(jsonText));

    BOOST_CHECK(importer.LoadMap(mapName));

    boost::filesystem::remove(mapName);
}

BOOST_FIXTURE_TEST_CASE(ShouldNotProcessMapWithInvalidTilePositions, JsonImporterFixture)
{
    std::string jsonText ="{ \"height\":6,\n"
        "\"layers\":[\n"
             "{\n"
              "\"data\":[0, 0, 0],\n"
              "\"height\":6,\n"
              "\"name\":\"\u0421\u043b\u043e\u0439 \u0442\u0430\u0439\u043b\u043e\u0432 1\",\n"
              "\"opacity\":1,\n"
              "\"type\":\"tilelayer\",\n"
              "\"visible\":true,\n"
              "\"width\":4,\n"
              "\"x\":0,\n"
              "\"y\":0\n"
             "}],\n"
        "\"nextobjectid\":1,\n"
        "\"orientation\":\"orthogonal\",\n"
        "\"renderorder\":\"left-up\",\n"
        "\"tileheight\":25,\n"
        "\"tilesets\":[\n"
        "{\n"
              "\"firstgid\":1,\n"
              "\"source\":\"untitled.tsx\"\n"
             "}],\n"
        "\"tilewidth\":25,\n"
        "\"type\":\"map\",\n"
        "\"version\":\"2017.05.16\",\n"
        "\"width\":4\n"
        "}\n";

    const std::string mapName = "invalid_tile_positions.json";

    CreateMapFile(mapName, std::move(jsonText));

    BOOST_CHECK(importer.LoadMap(mapName)== false);

    boost::filesystem::remove(mapName);
}

BOOST_FIXTURE_TEST_CASE(ShouldNotProcessMapWithInvalidParams, JsonImporterFixture)
{
    std::string jsonText ="{ \"height\":6,\n"
        "\"layers\":[\n"
             "{\n"
              "\"data\":[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0],\n"
              "\"height\":-1,\n"
              "\"name\":\"\u0421\u043b\u043e\u0439 \u0442\u0430\u0439\u043b\u043e\u0432 1\",\n"
              "\"opacity\":1,\n"
              "\"type\":\"tilelayer\",\n"
              "\"visible\":true,\n"
              "\"width\":-20,\n"
              "\"x\":0,\n"
              "\"y\":0\n"
             "}],\n"
        "\"nextobjectid\":1,\n"
        "\"orientation\":\"orthogonal\",\n"
        "\"renderorder\":\"left-up\",\n"
        "\"tileheight\":-1,\n"
        "\"tilesets\":[\n"
        "{\n"
              "\"firstgid\":1,\n"
              "\"source\":\"untitled.tsx\"\n"
             "}],\n"
        "\"tilewidth\":-1,\n"
        "\"type\":\"map\",\n"
        "\"version\":\"2017.05.16\",\n"
        "\"width\":4\n"
        "}\n";

    const std::string mapName = "invalid_params.json";

    CreateMapFile(mapName, std::move(jsonText));

    BOOST_CHECK(importer.LoadMap(mapName) == false);

    boost::filesystem::remove(mapName);
}

BOOST_FIXTURE_TEST_CASE(ShouldNotProcessMapWithInvalidJsonFormat, JsonImporterFixture)
{
    std::string jsonText ="This is not a json format.";

    const std::string mapName = "invalid_json.json";

    CreateMapFile(mapName, std::move(jsonText));

    BOOST_CHECK(importer.LoadMap(mapName) == false);

    boost::filesystem::remove(mapName);
}

BOOST_FIXTURE_TEST_CASE(ShouldNotProcessMapIfDoesntExist, JsonImporterFixture)
{
    std::string jsonText ="Some json text.";


    const std::string mapName = "map.json";
    const std::string noMapName = "no_file.json";

    CreateMapFile(mapName, std::move(jsonText));

    BOOST_CHECK(importer.LoadMap(noMapName) == false);

    boost::filesystem::remove(mapName);
}

BOOST_FIXTURE_TEST_CASE(ShouldNotProcessMapWithEmptyLayerOrTilePositions, JsonImporterFixture)
{
    std::string jsonText ="{ \"height\":6,\n"
        "\"layers\":[\n"
             "{\n"
              "\"height\":6,\n"
              "\"name\":\"\u0421\u043b\u043e\u0439 \u0442\u0430\u0439\u043b\u043e\u0432 1\",\n"
              "\"opacity\":1,\n"
              "\"type\":\"tilelayer\",\n"
              "\"visible\":true,\n"
              "\"width\":4,\n"
              "\"x\":0,\n"
              "\"y\":0\n"
             "}],\n"
        "\"nextobjectid\":1,\n"
        "\"orientation\":\"orthogonal\",\n"
        "\"renderorder\":\"left-up\",\n"
        "\"tileheight\":25,\n"
        "\"tilesets\":[\n"
        "{\n"
              "\"firstgid\":1,\n"
              "\"source\":\"untitled.tsx\"\n"
             "}],\n"
        "\"tilewidth\":25,\n"
        "\"type\":\"map\",\n"
        "\"version\":\"2017.05.16\",\n"
        "\"width\":4\n"
        "}\n";

    const std::string mapName = "layer-tile_pos-absent.json";

    CreateMapFile(mapName, std::move(jsonText));

    BOOST_CHECK(importer.LoadMap(mapName) == false);

    boost::filesystem::remove(mapName);
}

BOOST_AUTO_TEST_SUITE_END()

}

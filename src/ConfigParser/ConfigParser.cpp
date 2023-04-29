#include "ConfigParser.h"

ConfigParser::ConfigParser(std::string filePath) : _filePath(std::move(filePath))
{}

std::vector<threadContent> ConfigParser::getThreads() try
{
    std::vector<threadContent> threads;
    auto doc = getParsedDocument();

    // read threads array
    for (const auto& threadArr : doc["TRTThread"].GetArray())
    {
        threadContent thread;
        thread.number = threadArr["number"].GetInt();

        // read objects array
        for (const auto& objectArr : threadArr["objects"].GetArray())
        {
            objectContent object;
            object.className = objectArr["className"].GetString();
            object.varName = objectArr["varName"].GetString();
            object.count = objectArr["count"].GetInt();

            // read args array
            for (const auto& arg : objectArr["args"].GetArray())
            {
                if ( arg.IsInt() )
                {
                    object.args.emplace_back( std::to_string(arg.GetInt()) );
                }
                else
                {
                    object.args.emplace_back( arg.GetString() );
                }
            }

            thread.objects.emplace_back( std::move(object) );
        }

        threads.emplace_back( std::move(thread) );
    }

    return threads;
}
catch (...)
{
    throw;
}

std::vector<actionContent> ConfigParser::getActions()
{
    std::vector<actionContent> actions;
    auto doc = getParsedDocument();

    // read actions array
    for (const auto& actionObject : doc["actions"].GetArray())
    {
        actionContent action;
        action.action = actionObject["action"].GetString();

        if ( actionObject.HasMember("className") )
        {
            action.className = actionObject["className"].GetString();
        }

        if ( actionObject.HasMember("varName") )
        {
            action.varName = actionObject["varName"].GetString();
        }

        if ( actionObject.HasMember("funcName") )
        {
            action.funcName = actionObject["funcName"].GetString();
        }

        if ( actionObject.HasMember("args") )
        {
            for (const auto& arg : actionObject["args"].GetArray())
            {
                if ( arg.IsInt() )
                {
                    action.args.emplace_back( std::to_string(arg.GetInt()) );
                }
                else
                {
                    action.args.emplace_back( arg.GetString() );
                }
            }
        }

        actions.emplace_back( std::move(action) );
    }

    return actions;
}

void ConfigParser::writeConfig(const std::vector<threadContent>& threads, const std::vector<actionContent>& actions) try
{
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();

    // write threads
    rapidjson::Value threadArr(rapidjson::kArrayType);

    for (const auto& thread : threads)
    {
        rapidjson::Value threadObject(rapidjson::kObjectType);
        threadObject.AddMember("number", thread.number, doc.GetAllocator());

        rapidjson::Value objectArr(rapidjson::kArrayType);

        for (const auto& object : thread.objects)
        {
            rapidjson::Value objectVal(rapidjson::kObjectType);
            objectVal.AddMember("className", getValue(object.className, alloc), alloc);
            objectVal.AddMember("varName", getValue(object.varName, alloc), alloc);
            objectVal.AddMember("count", object.count, alloc);

            rapidjson::Value argsArr(rapidjson::kArrayType);

            for (auto& arg : object.args)
            {
                argsArr.PushBack(getValue(arg, alloc), alloc);
            }

            objectVal.AddMember("args", argsArr, alloc);
            objectArr.PushBack(objectVal, alloc);
        }

        threadObject.AddMember("objects", objectArr, alloc);
        threadArr.PushBack(threadObject, alloc);
    }

    doc.AddMember("TRTThread", threadArr, alloc);

    // write actions
    rapidjson::Value actionArr(rapidjson::kArrayType);

    for (auto& action : actions)
    {
        rapidjson::Value actionObject(rapidjson::kObjectType);
        actionObject.AddMember("action", getValue(action.action, alloc), alloc);

        if ( !action.className->isEmpty() )
        {
            actionObject.AddMember("className", getValue(action.className, alloc), alloc);
        }

        if ( !action.varName->isEmpty() )
        {
            actionObject.AddMember("varName", getValue(action.varName, alloc), alloc);
        }

        if ( !action.funcName->isEmpty() )
        {
            actionObject.AddMember("funcName", getValue(action.funcName, alloc), alloc);
        }

        if ( !action.args.empty() )
        {
            rapidjson::Value argsArr(rapidjson::kArrayType);

            for (const auto& arg : action.args)
            {
                argsArr.PushBack(getValue(arg, alloc), alloc);
            }

            actionObject.AddMember("args", argsArr, alloc);
        }

        actionArr.PushBack(actionObject, alloc);
    }

    doc.AddMember("actions", actionArr, alloc);

    // write json to file
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::ofstream file("/home/danila/Downloads/test");

    if ( !file.is_open() )
    {
        throw std::runtime_error(std::string("can't open file: ") + _filePath);
    }

    file << buffer.GetString();
}
catch (...)
{
    throw;
}

rapidjson::Document ConfigParser::getParsedDocument()
{
    rapidjson::Document doc;

    // read file to string stream
    std::ifstream file( _filePath );

    if ( !file.is_open() )
    {
        throw std::runtime_error(std::string("can't open file: ") + _filePath);
    }

    std::ostringstream sstr;
    sstr << file.rdbuf();

    doc.Parse( sstr.str().data() );
    return doc;
}

rapidjson::Value ConfigParser::getValue(const UniversalString& str, rapidjson::Document::AllocatorType& allocator)
{
    return {str->toStdString().data(), allocator};
}
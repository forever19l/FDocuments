#include "XmlHelper.h"
#include <sstream>
#include <string.h>
#include "Util.h"
#include <iostream>
#include <sstream>

const std::string XML_HEAD = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";

#ifdef WIN32
#define stricmp _stricmp
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
XmlNode::XmlNode()
{
	_node = NULL;
}

XmlNode::XmlNode(const XmlNode& other)
{
	_node = other._node;
	_file = other._file;
	_doc = other._doc;
}

XmlNode::~XmlNode()
{
	_node = NULL;
}

XmlNode& XmlNode::operator =(const XmlNode &other)
{
	if (this == &other)
		return *this;

	_node = other._node;
	_file = other._file;
	_doc = other._doc;
	return *this;
}

XmlNode::XmlNode(const std::shared_ptr<rapidxml::file<char> >& file,
			     const std::shared_ptr<rapidxml::xml_document<char> >& doc,
				 rapidxml::xml_node<char>* node)
{
	if (NULL != node)
	{
		_node = node;
		_file = file;
		_doc = doc;
	}
}

void XmlNode::SetXmlData(const std::shared_ptr<rapidxml::file<char> >& file,
						 const std::shared_ptr<rapidxml::xml_document<char> >& doc)
{
	_file = file;
	_doc = doc;
	_node = _doc->document();
}


bool XmlNode::ParseXml(const std::string& fileName, XmlNode& node)
{
	try
	{
		std::shared_ptr<rapidxml::file<char> > file(new rapidxml::file<char>(fileName.c_str()));
		std::shared_ptr<rapidxml::xml_document<char> > doc(new rapidxml::xml_document<char>());
		doc->parse<0>(file->data());

		node.SetXmlData(file, doc);
	}
	catch (std::exception ex)
	{
		return false;
	}

	return true;
}

XmlNode XmlNode::AllocateDocment()
{
	std::shared_ptr<rapidxml::xml_document<char> > doc(new rapidxml::xml_document<char>());
	XmlNode node;
	node.SetXmlData(NULL, doc);
	return node;
}

bool XmlNode::isValid() const
{
	//要校验document是否为NULL,不允许出现不属于document的node节点。(不属于document节点的node类似于野指针，危险)
	return (_node != NULL && _node->document() != NULL);
}

//取它的子节点列表
std::vector<XmlNode> XmlNode::GetChilds() const
{
	assert(_node != NULL);
	std::vector<XmlNode> result;
	if (isValid())
	{
		rapidxml::xml_node<>* firstChild = _node->first_node();
		while (firstChild != NULL)
		{
			if (firstChild->type() == rapidxml::node_element)
				result.push_back(XmlNode(_file, _doc, firstChild)); //只加元素节点

			firstChild = firstChild->next_sibling();
		}
	}

	return result;
}

//获取指定名称的子节点列表, name可以带路径的 如A.B.C
std::vector<XmlNode> XmlNode::GetChilds(const std::string& name) const
{
	assert(_node != NULL);

	std::vector<std::string> splitResults;
	Util::Split(name, ".", splitResults);

	std::vector<XmlNode> result;
	if (splitResults.size() == 1)
	{
		rapidxml::xml_node<>* firstChild = _node->first_node();
		while (firstChild != NULL)
		{
			if (firstChild->type() == rapidxml::node_element && 0 == strcmp(firstChild->name(), name.c_str()))
				result.push_back(XmlNode(_file, _doc, firstChild)); //只加元素节点

			firstChild = firstChild->next_sibling();
		}
	}
	else{
		rapidxml::xml_node<char>* parentNode = _node;
		std::vector<std::string>::const_iterator it = splitResults.begin();
		for (; it != splitResults.end(); it++)
		{
			const std::string& item = *it;
			if (parentNode == NULL)
				break;

			parentNode = parentNode->first_node(item.c_str());
		}
		if (parentNode != NULL)
		{
			parentNode = parentNode->parent();
			rapidxml::xml_node<>* firstChild = parentNode->first_node();
			std::string childName = splitResults[splitResults.size() - 1];
			while (firstChild != NULL)
			{
				if (firstChild->type() == rapidxml::node_element && 0 == strcmp(firstChild->name(),childName.c_str()))
					result.push_back(XmlNode(_file, _doc, firstChild)); //只加元素节点

				firstChild = firstChild->next_sibling();
			}
		}
	}

	return result;
}

//获取它的第一个子节点名。 （childName可以是带路径的，如A.B.C）
XmlNode XmlNode::Child(const std::string& childName) const
{
	assert(_node != NULL);

	std::vector<std::string> splitResults;
	Util::Split(childName, ".", splitResults);
	rapidxml::xml_node<char>* parentNode = _node;

	std::vector<std::string>::const_iterator it = splitResults.begin();
	for (; it != splitResults.end(); it++)
	{
		const std::string& item = *it;
		if (parentNode == NULL)
			break;

		parentNode = parentNode->first_node(item.c_str());
	}

	if (parentNode != NULL)
	{
		return XmlNode(_file, _doc, parentNode);
	}

	return XmlNode();
}

//获取它的父节点
XmlNode XmlNode::Parent() const
{
	assert(_node != NULL);

	return XmlNode(_file, _doc, _node->parent());
}

//获取子节点个数
int XmlNode::ChildCount() const
{
	assert(_node != NULL);
	int count = 0;
	rapidxml::xml_node<>* firstChild = _node->first_node();
	while (firstChild != NULL)
	{
		count++;
		firstChild = firstChild->next_sibling();
	}

	return count;
}

//获取指定索引的子节点
XmlNode XmlNode::Child(int index) const
{
	assert(_node != NULL);
	int count = 0;
	rapidxml::xml_node<>* firstChild = _node->first_node();
	while (firstChild != NULL)
	{
		if (count == index)
			break;

		count++;
		firstChild = firstChild->next_sibling();
	}

	return firstChild != NULL ? XmlNode(_file, _doc, firstChild) : XmlNode();
}

std::string XmlNode::Name() const
{
	assert(_node != NULL);
	return _node->name();
}

//获取属性个数
int XmlNode::AttrCount() const
{
	assert(_node != NULL);
	int count = 0;
	rapidxml::xml_attribute<>* firstAttr = _node->first_attribute();
	while (firstAttr != NULL)
	{
		count++;
		firstAttr = firstAttr->next_attribute();
	}

	return count;
}

//判断当前节点的属性值是否存在
bool XmlNode::IsExistAttribute(const std::string& attrName) const
{
	assert(_node != NULL);
	rapidxml::xml_attribute<>* firstAttr = _node->first_attribute();
	while (firstAttr != NULL)
	{
#ifdef WIN32
		if (0 == stricmp(attrName.c_str(), firstAttr->name()))
			break;
#else
        if (0 == strcasecmp(attrName.c_str(), firstAttr->name()))
            break;
#endif
		firstAttr = firstAttr->next_attribute();
	}

	return (firstAttr == NULL) ? false : true;
}

XmlNode XmlNode::AddChild_(const std::string& name, const std::string& value) const
{
	assert(_node != NULL);
	rapidxml::xml_node<>* child = _doc->allocate_node(rapidxml::node_element, 
		_doc->allocate_string(name.c_str()), _doc->allocate_string(value.c_str()));
	child->name(_doc->allocate_string(name.c_str()));
	
	_node->append_node(child);
	return XmlNode(_file, _doc, child); //返回子节点
}

void XmlNode::SetValue_(const std::string& value)
{
	assert(_node != NULL);
	_node->value(_doc->allocate_string(value.c_str()));
	if (_node->first_node() != NULL)
	{
		_node->first_node()->value(_doc->allocate_string(value.c_str()));
	}
}

void XmlNode::SetAttribute_(const std::string& attrName, const std::string& attrValue)
{
	assert(_node != NULL);
	if (!IsExistAttribute(attrName))
	{//不存在该属性
		rapidxml::xml_attribute<>* attr = _doc->allocate_attribute(_doc->allocate_string(attrName.c_str()), 
																   _doc->allocate_string(attrValue.c_str()));
		_node->append_attribute(attr);
	}
	else
	{//已存在该属性
		rapidxml::xml_attribute<>* firstAttr = _node->first_attribute(attrName.c_str());
		firstAttr->value(_doc->allocate_string(attrValue.c_str()));
	}
}

void XmlNode::Save(std::basic_ostream<char> &stream) const
{
    rapidxml::print(stream, *_node, 0);
}

bool XmlNode::Save(const std::string& fileName)
{
	assert(_node != NULL);
	if (_node == NULL)
		return false;

    std::basic_stringstream<char> stream;
    stream << XML_HEAD;
    Save(stream);

    std::ofstream fileStream(fileName.c_str(), std::ios_base::binary);
    fileStream << (char)0xef << (char)0xbb << (char)0xbf;
    fileStream.write(stream.str().c_str(), stream.str().length());
    fileStream.close();

    /*std::string stream;
    stream += XML_HEAD;
    //rapidxml::print(stream, *_node);

    std::basic_stringstream<char> stream1;
    stream1 << XML_HEAD;
    //rapidxml::print(stream1, *_node, 0);

    std::ofstream out(fileName.c_str());//ofstream 默认时，如果文件存在则会覆盖原来的内容，不存在则会新建
    out << _node;
    out.close();*/

    //std::ofstream fileStream(fileName.c_str(), std::ios_base::binary);
    //fileStream << (char)0xef << (char)0xbb << (char)0xbf;
    //fileStream.write(stream.c_str(), stream.length());
    //fileStream.close();

	return true;
}

bool XmlNode::SaveToStream(std::string& stream)
{
	assert(_node != NULL);
	if (_node == NULL)
		return false;

	//stream += XML_HEAD;
   // rapidxml::print(std::back_inserter(stream), *_node, 0);

	std::basic_stringstream<char> tstream;
	tstream << XML_HEAD;
	Save(tstream);

	stream.assign(tstream.rdbuf()->str());
	return true;
}

bool XmlNode::LoadFromStream(const std::string& stream)
{
	try
	{
		std::istringstream sss(stream);//需要转为输入流

		_file = std::shared_ptr<rapidxml::file<char>>(new rapidxml::file<char>(sss));
		std::shared_ptr<rapidxml::file<char> > ();
		_doc = std::shared_ptr<rapidxml::xml_document<char>>(new rapidxml::xml_document<char>());

		_doc->parse<0>(_file->data());
		_node = _doc->document();
		if (_node == NULL)
			return false;
		return true;
	}
	catch (std::exception ex)
	{
		return false;
	}

	return true;
}

void XmlNode::CreateEmptyXmlFile(const std::string& fileName)
{
	std::string stream;
	stream += XML_HEAD;

	std::ofstream fileStream(fileName.c_str(), std::ios_base::binary);
	fileStream << (char)0xef << (char)0xbb << (char)0xbf;
	fileStream.write(stream.c_str(), stream.length());
	fileStream.close();
}

//删除指定位置的子节点
void XmlNode::RemoveChild(int index) const
{
	int count = 0;
	rapidxml::xml_node<>* firstChild = _node->first_node();
	while (firstChild != NULL)
	{
		if (index == count)
			break;

		count++;
		firstChild = firstChild->next_sibling();
	}

	if (firstChild != NULL)
		_node->remove_node(firstChild);
}

//删除所有子节点
void XmlNode::RemoveAllChild() const
{
	_node->remove_all_nodes();
}

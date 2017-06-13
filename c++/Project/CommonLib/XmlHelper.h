#ifndef XMLHELPER_H
#define XMLHELPER_H

/***************************************************************
 * Purpose:   rapid xml解析封装类，方便读写。
 * Author:    wuzx ()
 * Created:   2016-06-03
 **************************************************************/

#include <string>
#include <memory>
#include <vector>
#include <assert.h>

#include "rapidXml/rapidxml.hpp"
#include "rapidXml/rapidxml_utils.hpp"
#include "rapidXml/rapidxml_print.hpp"
#include "ConvertHelper.h"
#include <string.h>

#ifdef WIN32
#define stricmp _stricmp
#endif
class XmlNode
{
public:

	//从文件解析xml
	static bool ParseXml(const std::string& fileName, XmlNode& node);
	//创建一个文档节点
	static XmlNode AllocateDocment();

	static void CreateEmptyXmlFile(const std::string& fileName); //创建空的xml文件

	XmlNode();
	XmlNode(const XmlNode& tree);
	~XmlNode();
    XmlNode& operator =(const XmlNode &other);

	//是否合法节点
	bool isValid() const;

	//取它的子节点列表
	std::vector<XmlNode> GetChilds() const;

	//获取指定名称的子节点列表, name可以带路径的 如A.B.C
	std::vector<XmlNode> GetChilds(const std::string& name) const;

	//获取它的第一个子节点名。 （childName可以是带路径的，如A.B.C）
	XmlNode Child(const std::string& childName) const;

	//获取它的父节点
	XmlNode Parent() const;

	//获取子节点个数
	int ChildCount() const;
	//获取指定索引的子节点
	XmlNode Child(int index) const;

	//返回节点值,目前模板类型支持string,int,float,double,bool,long,long long。
	template <typename T>
	T Value(T t = T()) const
	{
		assert(_node != NULL);
		return ConvertHelper::Convert<T>(_node->value(), t);
	}

	//xml节点名称
	std::string Name() const;

	//获取属性个数
	int AttrCount() const;

	//获取当前节点的属性值。目前模板类型支持wstring,string,int,float,double,bool,long,long long。
	template<typename T>
	T Attribute(const std::string& attrName, T t = T()) const
	{
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

		return firstAttr == NULL ? t : ConvertHelper::Convert<T>(firstAttr->value(), t);
	}

	//判断当前节点的属性值是否存在
	bool IsExistAttribute(const std::string& attrName) const;

	//////////////////////////////////写操作 begin/////////////////////////////////////////////////////////////////////
	//新增子节点并设置它的值。目前模板类型支持string,int,float,double,bool,long,long long。
	template<typename T=std::string>
	XmlNode AddChild(const std::string& name, const T& value = T())
	{
		return AddChild_(name, ConvertHelper::ConvertToStr(value));
	}

	//设置当前节点的值,目前模板类型支持string,int,float,double,bool,long,long long。
	template <typename T>
	void SetValue(const T& value)
	{
		SetValue_(ConvertHelper::ConvertToStr(value));
	}

	//设置当前节点的属性值。如果属性不存在，则新增。如果存在，则直接修改它的值。
	template<typename T>
	void SetAttribute(const std::string& attrName, const T& attrValue)
	{
		SetAttribute_(attrName, ConvertHelper::ConvertToStr(attrValue));
	}

	//删除指定位置的子节点
	void RemoveChild(int index) const;

	//删除所有子节点
	void RemoveAllChild() const;

	//把节点保存为文件
	bool Save(const std::string& fileName);
    void Save(std::basic_ostream<char> &stream) const;
	bool SaveToStream(std::string& stream); //把节点内容保存成string
	bool LoadFromStream(const std::string& stream); //从xml流里读取节点信息
	//////////////////////////////////写操作 end/////////////////////////////////////////////////////////////////////

private:
	XmlNode(const std::shared_ptr<rapidxml::file<char> >& file,
		const std::shared_ptr<rapidxml::xml_document<char> >& doc,
		rapidxml::xml_node<char>* node);
	void SetXmlData(const std::shared_ptr<rapidxml::file<char> >& file,
		const std::shared_ptr<rapidxml::xml_document<char> >& doc);

	XmlNode AddChild_(const std::string& name, const std::string& value) const;
	void SetValue_(const std::string& value);
	void SetAttribute_(const std::string& attrName, const std::string& attrValue);
private:
	std::shared_ptr<rapidxml::file<char>> _file;
	std::shared_ptr<rapidxml::xml_document<char>> _doc;

	rapidxml::xml_node<char>* _node;
};

#endif // XMLHELPER_H

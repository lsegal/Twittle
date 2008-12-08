#pragma once

/**
 * Basic interface to perform serialization on an object to any output.
 * Implement this class by specializing the template for the types
 * S and T where S represents the path representation and T represents 
 * the class to be serialized. For file based serialization, S can be
 * the filesystem path represented as a std::string or wxString
 *
 * @see Serializer<wxString,TwitterFeed>
 */
template<typename S, typename T>
class Serializer
{
private:
	Serializer() { }
	~Serializer() { }
public:
	/**
	 * Performs deserialization from a resource locator to an object.
	 *
	 * @param path the location of the serialized object to read
	 * @return the deserialized object
	 */
	static T *Read(const S& path);

	/**
	 * Performs serialization on an object to resource locator.
	 *
	 * @param path the location of the serialized object to write to
	 * @param obj the object to serialize
	 */
	static void Write(const S& path, const T& obj);
};

#ifndef GTB_TIME_SERIES_INCLUDED
#define GTB_TIME_SERIES_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_SUBJECT_INCLUDED
#include "Subject.hpp"
#endif

#ifndef GTB_ASSERTIONS_INCLUDED
#include "assertions.hpp"
#endif

#ifndef GTB_IO_UTIL_INCLUDED
#include "io_util.hpp"
#endif

#ifndef GTB_CXX_VECTOR_INCLUDED
#include <vector>
#define GTB_CXX_VECTOR_INCLUDED
#endif

#ifndef GTB_CXX_STRING_INCLUDED
#include <string>
#define GTB_CXX_STRING_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


template <class T>
class Time_series : public Subject {
public:
	explicit Time_series(const char *file_name);
	explicit Time_series(std::vector<T> &values);

 	class File_header {
	public:
		File_header(unsigned num_times, 
			    unsigned num_values,
			    const T &min_value,
			    const T &max_value);

		File_header(FILE *fp);

		void write(FILE *fp) const;

		unsigned get_num_times() const;
		unsigned get_num_values() const;
		const T &get_min_value() const;
		const T &get_max_value() const;

		void set_min_value(const T &t);
		void set_max_value(const T &t);

	protected:
		unsigned m_num_times; 
		unsigned m_num_values;
		T m_min_value;
		T m_max_value;
	};
	
	unsigned get_num_times() const;
	unsigned get_current_time() const;
	unsigned get_num_values() const;
	const T &get_min_value() const;
	const T &get_max_value() const;
	void set_min_value(const T &t) { m_min_value = t; }
	void set_max_value(const T &t) { m_max_value = t; }
	const T &get_value(unsigned i) const;
	const std::vector<T> &get_values() const;
	template <class F> void transform_values(F f);

        void go_to_first_time();
        void go_to_last_time();
        void go_to_next_time();
        void go_to_previous_time();
	void go_to_time(unsigned t);

protected:
	void read_values();

	std::string m_file_name;
	unsigned m_num_times;
	T m_min_value;
	T m_max_value;
	unsigned m_current_time;
	std::vector<T> m_values;	
};


typedef Time_series<float> Scalar_time_series;

class Vector_3f;
typedef Time_series<Vector_3f> Vector_time_series;


template <class T>
Time_series<T>::Time_series(const char *file_name)
	: m_file_name(file_name)
{
	FILE *fp;
	GTB_FOPEN(fp, m_file_name.c_str(), "rb");
	File_header header(fp);
	fclose(fp);

	m_num_times = header.get_num_times();
	m_values.resize(header.get_num_values());
	m_min_value = header.get_min_value();
	m_max_value = header.get_max_value();

	go_to_first_time();
}


template <class T>
Time_series<T>::Time_series(std::vector<T> &values)
	: m_num_times(1),
	  m_current_time(0),
	  m_values(values)
{
}


template <class T>
Time_series<T>::File_header::File_header(unsigned num_times, 
					 unsigned num_values,
					 const T &min_value,
					 const T &max_value)
	: m_num_times(num_times),
	  m_num_values(num_values),
	  m_min_value(min_value),
	  m_max_value(max_value)
{
}


template <class T>
Time_series<T>::File_header::File_header(FILE *fp)
{
	unsigned n = 0;
	n += fread(&m_num_times, sizeof(m_num_times), 1, fp);
	n += fread(&m_num_values, sizeof(m_num_values), 1, fp);
	n += fread(&m_min_value, sizeof(m_min_value), 1, fp);
	n += fread(&m_max_value, sizeof(m_max_value), 1, fp);
	if (n != 4) {
		GTB_ERROR("failed to read header");
	}
}


template <class T>
void Time_series<T>::File_header::write(FILE *fp) const
{
	unsigned n = 0;
	n += fwrite(&m_num_times, sizeof(m_num_times), 1, fp);
	n += fwrite(&m_num_values, sizeof(m_num_values), 1, fp);
	n += fwrite(&m_min_value, sizeof(m_min_value), 1, fp);
	n += fwrite(&m_max_value, sizeof(m_max_value), 1, fp);
	if (n != 4) {
		GTB_ERROR("failed to write header");
	}
}


template <class T>
unsigned Time_series<T>::File_header::get_num_times() const
{
	return m_num_times;
}


template <class T>
unsigned Time_series<T>::File_header::get_num_values() const
{
	return m_num_values;
}


template <class T>
const T &Time_series<T>::File_header::get_min_value() const
{
	return m_min_value;
}


template <class T>
const T &Time_series<T>::File_header::get_max_value() const
{
	return m_max_value;
}


template <class T>
void Time_series<T>::File_header::set_min_value(const T &t)
{
	m_min_value = t;
}


template <class T>
void Time_series<T>::File_header::set_max_value(const T &t)
{
	m_max_value = t;
}


template <class T>
unsigned Time_series<T>::get_num_times() const
{
	return m_num_times;
}


template <class T>
unsigned Time_series<T>::get_current_time() const
{
	return m_current_time;
}


template <class T>
unsigned Time_series<T>::get_num_values() const
{
	return m_values.size();
}


template <class T>
const T &Time_series<T>::get_min_value() const
{
	return m_min_value;
}


template <class T>
const T &Time_series<T>::get_max_value() const
{
	return m_max_value;
}


template <class T>
const T &Time_series<T>::get_value(unsigned i) const
{
    //printf("%d %d\n", i, m_values.size());
	GTB_REQUIRE(i < m_values.size());
	return m_values[i];
}


template <class T>
const std::vector<T> &Time_series<T>::get_values() const
{
	return m_values;
}


template <class T>
template <class F>
void Time_series<T>::transform_values(F f)
{
	for (unsigned i = 0; i < m_values.size(); ++i) {
		m_values[i] = f(m_values[i]);
	}
	notify();
}


template <class T>
void Time_series<T>::go_to_first_time()
{
	m_current_time = 0;
	read_values();
}


template <class T>
void Time_series<T>::go_to_last_time()
{
	m_current_time = m_num_times - 1;
	read_values();
}


template <class T>
void Time_series<T>::go_to_next_time()
{
	m_current_time = (m_current_time + 1) % m_num_times;
	read_values();
}


template <class T>
void Time_series<T>::go_to_previous_time()
{
	m_current_time = (m_current_time + m_num_times - 1) % m_num_times;
	read_values();
}


template <class T>
void Time_series<T>::go_to_time(unsigned t)
{
	GTB_REQUIRE(t < m_num_times);
	m_current_time = t;
	read_values();
}


template <class T>
void Time_series<T>::read_values()
{
	GTB_CHECK(m_values.size() > 0);
	GTB_CHECK(m_current_time < m_num_times);
	GTB_CHECK(m_file_name.size() != 0);
	FILE *fp;
	GTB_FOPEN(fp, m_file_name.c_str(), "rb");
	long offset = sizeof(File_header)
		+ m_current_time * m_values.size() * sizeof(T);
	fseek(fp, offset, SEEK_SET);
	if (fread(&m_values[0], m_values.size() * sizeof(T), 1, fp) != 1) {
		GTB_ERROR("failed to read values");
	}
	fclose(fp);

	notify();
}


GTB_END_NAMESPACE

#endif // GTB_TIME_SERIES_INCLUDED

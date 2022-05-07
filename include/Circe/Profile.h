#include <string>
#include <chrono>
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <memory>
#include <map>
#include <sstream>
#include <fstream>
#include <list>
#include <csignal>
#include <cstring>
#include <cstddef>

//Profiler inspired by:
//https://github.com/grant-zietsman/rapid-profile/blob/master/rapid-profile.hpp

/*
#define CIRCE_STRING_ID(text) std::hash<std::string>{}(text)
#define CIRCE_INFO(text) {Circe::log.print("INFO", __FILE__, __LINE__, text);}
#define CIRCE_WARNING(text) {Circe::log.print("WARNING", __FILE__, __LINE__, text);}
#define CIRCE_ERROR(text) {Circe::log.print("ERROR", __FILE__, __LINE__, text);}
#define CIRCE_PRINT(text) {Circe::log.print(text);}
*/

#define CIRCE_MARK {Circe::Debug::mark(__FILE__, __LINE__);}
#define CIRCE_BREAK {Circe::Debug::breakPoint(__FILE__, __LINE__);}


#define CIRCE_CONCAT(x, y) CIRCE_CONCAT2(x, y)
#define CIRCE_CONCAT2(x, y) x ## y


#define PROFILE_STRING_SIZE 64
#define PROFILE_CHUNK_SIZE 1048576
#define PROFILE_MAX_TIMERS 1024

#define CIRCE_NOW() Circe::ProfileType::clock::now()

#define CIRCE_PROF_ID(NAME) _circe_prof_##NAME##_id
#define CIRCE_PROF_INST(NAME) _circe_prof_##NAME


#define CIRCE_INIT_PROFILER() Circe::Profiler<PROFILE_STRING_SIZE>::init()
#define CIRCE_START_PROFILE(NAME)                               \
	static Circe::ProfileType::id CIRCE_PROF_ID(NAME) =    \
	   Circe::Profiler<PROFILE_STRING_SIZE>::getID(#NAME, __FILE__, __LINE__);\
	Circe::Profile& CIRCE_PROF_INST(NAME) =                     \
	   Circe::Profiler<PROFILE_STRING_SIZE>::getProfile();      \
	CIRCE_PROF_INST(NAME).id = CIRCE_PROF_ID(NAME);             \
	CIRCE_PROF_INST(NAME).start = CIRCE_NOW();                               


#define CIRCE_STOP_PROFILE(NAME) CIRCE_PROF_INST(NAME).stop = CIRCE_NOW()

	
/**
How to use profiler:

{
	CIRCE_INIT_PROFILER();
	CIRCE_START_PROFILE(name);
	CIRCE_END_PROFILE(name);
	...
}
**/

namespace Circe
{
	namespace ProfileType
	{
		typedef unsigned int id;
		typedef std::chrono::steady_clock clock;
		typedef std::chrono::duration<float> duration;
		typedef clock::time_point time_point;
	}
	
	template<class T>
	class chunker
	{
		public:
			chunker(std::size_t chunk_size):chunk_size_(chunk_size), 
											chunk_(NULL)
			{
				chunk_ = new std::vector<T>();
				chunk_->reserve(chunk_size_);
				chunks_.push_back(chunk_);

				rechunk();
			}

			~chunker()
			{
				 for (typename std::list<std::vector<T> *>::iterator it 
						 = chunks_.begin(); it != chunks_.end(); it++)
				 {
					 delete *it;
				 }
        		chunks_.clear();
			}

			T& push_back(T const& item = T())
			{
				if(chunk_->size() + 1 == chunk_size_) rechunk();
				chunk_->push_back(item);
				return chunk_->back();
			}

			std::list<std::vector<T>*>& chunks()
			{
				return chunks_;
			}

		protected:
			void rechunk()
			{
				std::vector<T>* prev_chunk = chunk_;
				chunk_ = chunks_.back();

				std::vector<T>* next = new std::vector<T>();
				next->reserve(chunk_size_);
				chunks_.push_back(next);
			}

		private:
			std::size_t chunk_size_;
			std::vector<T>* chunk_;
			std::list<std::vector<T>*> chunks_;

	};

	struct Profile
	{
		Profile()
		{}

		Profile(const ProfileType::id id): id(id), start(CIRCE_NOW())
		{}

		float getDuration() const
		{
			return ProfileType::duration(stop-start).count();
		}

		ProfileType::id id;
		ProfileType::time_point start;
		ProfileType::time_point stop;

		template<std::size_t N>
		struct tag
		{
			char name[N];
			char file[N];
			int line;
		};
	};


	template<std::size_t N>
	class Profiler
	{
		public:
			static void init()
			{
				start_time();
				tags().reserve(PROFILE_MAX_TIMERS);

				std::atexit(Profiler::exit);
				::signal(SIGINT, Profiler::signal);
			}


			static Profile& getProfile()
			{
				return profiles().push_back();
			}

			static ProfileType::id getID(const char* name, const char* file,
										 int line)
			{
				static ProfileType::id id = 0;
				tags().push_back(Profile::tag<N>());
				Profile::tag<N>& tag = tags().back();

				strncpy(tag.name, name, N);
				tag.name[N-1] = '\0';

				strncpy(tag.file, file, N);
				tag.file[N-1] = '\0';

				tag.line = line;

				return id++;
			}


		private:

			static void exit()
			{
				log();
			}

			static void signal(int signum)
			{
				::exit(signum);
			}

			static void log()
			{
				std::vector<Profile::tag<N>>& tags = Profiler::tags();
				std::list<std::vector<Profile>*>& chunks = profiles().chunks();

				std::cout << "id\tname\tfile\tline" << std::endl;
				
				for (typename std::vector<Profile::tag<N> >::iterator it = 
						tags.begin(); it != tags.end(); it++)
				{
					std::cout << (it - tags.begin()) << '\t' << it->name 
					<< '\t' << it->file << '\t' << it->line << std::endl;
				}

				std::cout << "id\tstart\tstop\tduration" << std::endl;
				for (std::list<std::vector<Profile> *>::iterator it = 
						chunks.begin(); it != chunks.end(); it++)
				{
					for (std::vector<Profile>::iterator vit = 
							(*it)->begin(); vit != (*it)->end(); vit++)
					{
						std::cout << vit->id << '\t' 
							<< relative(vit->start) * 1e6 
							<< '\t' << relative(vit->stop) * 1e6 << '\t'
							<< relative(vit->stop, vit->start) * 1e6 
							<< std::endl;
					}
				}
			}

			static float relative(ProfileType::time_point const & stop = 
									ProfileType::clock::now(),
								  ProfileType::time_point const & start =
								  	start_time())
			{
				return ProfileType::duration(stop-start).count();
			}

			static ProfileType::time_point const & start_time()
			{
				static ProfileType::time_point start_time = 
													CIRCE_NOW();
				return start_time;
			}

			static std::vector<Profile::tag<N>>& tags()
			{
				static std::vector<Profile::tag<N>> tags;
				return tags;
			}

			static chunker<Profile>& profiles()
			{
				static chunker<Profile>* profiles = 
					new chunker<Profile>(PROFILE_CHUNK_SIZE);
				return *profiles;
			}
			
	};

	class Debug
	{
		public:
			void mark(const char* file, const int line){
				std::string fileName(file);

				
				auto dt = ProfileType::duration(CIRCE_NOW() - timeStamp);
				timeStamp = CIRCE_NOW();
				
				std::string text = "[MARK "+std::to_string(markIndex)
									+": "+fileName+" - line "
									+std::to_string(line)+"]\t\t"
									+"Time since last mark = "
									+std::to_string(dt.count()/1000.0)+" ms.";
				std::cout << text << std::endl;
				markIndex++;
			}
			
			void breakPoint(const char* file, const int line){
				mark(file, line);
				std::cout << "Press any key to continue..." << std::endl;
				getchar();
			}
			
		private:
			unsigned int markIndex = 0;
			ProfileType::time_point timeStamp=CIRCE_NOW();
	};

	
	class Log
	{
		public:
			inline void print(const std::string& type, 
							  const char* file, 
							  const int line, 
							  const std::string text) const
			{
				std::string fileName(file);
				std::cout << ("["+fileName+": "+std::to_string(line)+"] "+type+": "+text+"\n") << std::endl;
			}

			template<typename T>
			void print(T& t)
			{
				std::cout << t;
			}
			
			template<std::size_t N, std::size_t M>
			void printArray(const char charArray[N][M]) const
			{
				for(int j = 0;j < M; j++){
					for(int i = 0;i < N; i++){
						std::cout << charArray[i][j];
					}
					std::cout << std::endl;
				}
			}
	};
	
	static Log log = Log();
	
}

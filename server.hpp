#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <iostream>
 
# include <netinet/in.h>
# include <libc.h>
 
# include <boost/smart_ptr/shared_ptr.hpp>
# include <boost/lambda/lambda.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
 
# include <boost/log/common.hpp>
# include <boost/log/expressions.hpp>
# include <boost/log/attributes.hpp>
# include <boost/log/sinks.hpp>
# include <boost/log/sources/logger.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace bll = boost::lambda;

typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

# define START		1
# define STOP		2

struct Client
{
	int sock;
	int registrFlag = 0;
	char message[1024];
	int bytes_read;
};

#endif

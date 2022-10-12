// #include <sys/types.h>
// #include <sys/socket.h>
// #include <iostream>

// #include <netinet/in.h>
// #include <libc.h>

// #include <boost/smart_ptr/shared_ptr.hpp>
// #include <boost/lambda/lambda.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>

// #include <boost/log/common.hpp>
// #include <boost/log/expressions.hpp>
// #include <boost/log/attributes.hpp>
// #include <boost/log/sinks.hpp>
// #include <boost/log/sources/logger.hpp>

#include "server.hpp"

// namespace logging = boost::log;
// namespace attrs = boost::log::attributes;
// namespace src = boost::log::sources;
// namespace sinks = boost::log::sinks;
// namespace expr = boost::log::expressions;
// namespace keywords = boost::log::keywords;
// namespace bll = boost::lambda;

// typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

void init_file_collecting(boost::shared_ptr< file_sink > sink)
{
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = "logs",                      /*< the target directory >*/
        keywords::max_size = 16 * 1024 * 1024,          /*< maximum total size of the stored files, in bytes >*/
        keywords::min_free_space = 100 * 1024 * 1024,   /*< minimum free space on the drive, in bytes >*/
        keywords::max_files = 512                       /*< maximum number of stored files >*/
    ));
}

// #define START		1
// #define STOP		2

// struct Client
// {
// 	int sock;
// 	int registrFlag = 0;
// 	char message[1024];
// 	int bytes_read;
// };

int	isThisStartMess(char *message)
{
	std::cout << message << "= message\n";
	if (strcmp(message, "START") == 0)
		return 0;
	return 1;
}

int	isThisStopMess(char *message)
{
	if (strcmp(message, "STOP") == 0)
		return 0;
	return 1;
}

void	checkClientMessage(Client *client, src::logger lg)
{
	if (isThisStopMess(client->message) == 0)
			client->registrFlag = STOP;
	if (client->registrFlag == 0)
	{
		if (isThisStartMess(client->message) == 0)
		{
			client->registrFlag = START;
			BOOST_LOG(lg) << "client registration";
		}
		else
		{
			BOOST_LOG(lg) << "warning: interaction protocol violation :" << client->message;
		}
	}
	else if (client->registrFlag == 1)
	{
		BOOST_LOG(lg) << client->message;
	}
}

void init_logging()
{
    boost::shared_ptr< file_sink > sink(new file_sink(
        keywords::file_name = "%Y%m%d_%H%M%S_%5N.xml",
        keywords::rotation_size = 16384
    ));

    init_file_collecting(sink);
    sink->locked_backend()->scan_for_files();
    sink->set_formatter
    (
        expr::format("\t<record id=\"%1%\" timestamp=\"%2%\">%3%</record>")
            % expr::attr< unsigned int >("RecordID")
            % expr::attr< boost::posix_time::ptime >("TimeStamp")
            % expr::xml_decor[ expr::stream << expr::smessage ]
    );

    sink->locked_backend()->set_open_handler
    (
        bll::_1 << "<?xml version=\"1.0\"?>\n<log>\n"
    );
    sink->locked_backend()->set_close_handler
    (
        bll::_1 << "</log>\n"
    );
    logging::core::get()->add_sink(sink);
}

int main()
{
	int			listener;
	struct		sockaddr_in addr;
	Client		client;
	src::logger lg;

	init_logging();
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
	logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0)
    {
        perror("socket");
        exit(1);
    }
	addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
	listen(listener, 1);

	client.sock = accept(listener, NULL, NULL);
	client.registrFlag = 0;
	if(client.sock < 0)
	{
		perror("accept");
		exit(3);
	}
    while(client.registrFlag != STOP)
    {
		client.bytes_read = recv(client.sock, client.message, 1024, 0);
		if (client.bytes_read > 0)
		{
			std::cout <<" bytes = " << client.bytes_read;
			client.message[client.bytes_read] = '\0';
			checkClientMessage(&client, lg);
		}
	}
	std::cout << "STOP and close\n";
	close(client.sock);
	return 0;
}

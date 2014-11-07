﻿//
// Copyright (C) 2013 Jack.
//
// Author: jack
// Email:  jack.wgm at gmail dot com
//

#pragma once

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "serialization.hpp"
#include "io_service_pool.hpp"
#include "connection.hpp"

namespace av_router {

	typedef boost::function<void(google::protobuf::Message*, connection_ptr, connection_manager&)> message_callback;
	typedef std::map<std::string, message_callback> message_callback_table;

	class server
		: public boost::noncopyable
	{
		friend class connection;
	public:
		explicit server(io_service_pool& ios, unsigned short port, std::string address = "0.0.0.0");
		~server();

	public:
		// 启动或停止.
		void start();
		void stop();

		// 添加或删除消息处理模块.
		bool add_message_process_moudle(const std::string& name, message_callback cb);
		bool del_message_process_moudle(const std::string& name);

	private:
		// 处理客户连接.
		void handle_accept(const boost::system::error_code& error);
		// 调用name所对应的消息处理模块.
		void do_message(google::protobuf::Message*, connection_ptr);

	private:
		io_service_pool& m_io_service_pool;
		boost::asio::io_service& m_io_service;
		boost::asio::ip::tcp::acceptor m_acceptor;
		connection_ptr m_connection;
		connection_manager m_connection_manager;
		boost::shared_mutex m_message_callback_mtx;
		message_callback_table m_message_callback;
	};

}

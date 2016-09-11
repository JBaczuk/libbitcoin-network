/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_NETWORK_SESSION_MANUAL_HPP
#define LIBBITCOIN_NETWORK_SESSION_MANUAL_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/channel.hpp>
#include <bitcoin/network/connector.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/sessions/session_batch.hpp>
#include <bitcoin/network/settings.hpp>

namespace libbitcoin {
namespace network {
    
class p2p;

/// Manual connections session, thread safe.
class BCT_API session_manual
  : public session_batch, track<session_manual>
{
public:
    typedef std::shared_ptr<session_manual> ptr;
    typedef std::function<void(const code&, channel::ptr)> channel_handler;

    /// Construct an instance.
    session_manual(p2p& network, bool notify_on_connect);

    /// Start the manual session.
    void start(result_handler handler) override;

    /// Maintain connection to a node.
    virtual void connect(const std::string& hostname, uint16_t port);

    /// Maintain connection to a node with callback on first connect.
    virtual void connect(const std::string& hostname, uint16_t port,
        channel_handler handler);

protected:
    /// Overridden to implement pending outbound channels.
    void start_channel(channel::ptr channel,
        result_handler handle_started) override;

    /// Override to attach specialized protocols upon channel start.
    virtual void attach_protocols(channel::ptr channel);

private:
    void start_connect(const std::string& hostname, uint16_t port,
        channel_handler handler, uint32_t retries);

    void handle_started(const code& ec, result_handler handler);
    void handle_connect(const code& ec, channel::ptr channel,
        const std::string& hostname, uint16_t port,
        channel_handler handler, uint32_t retries);

    void handle_channel_start(const code& ec, const std::string& hostname,
        uint16_t port, channel::ptr channel, channel_handler handler);
    void handle_channel_stop(const code& ec, const std::string& hostname,
        uint16_t port);

    bc::atomic<connector::ptr> connector_;
};

} // namespace network
} // namespace libbitcoin

#endif

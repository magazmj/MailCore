/*
 * libEtPan! -- a mail stuff library
 *
 * Copyright (C) 2001, 2005 - DINH Viet Hoa
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the libEtPan! project nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * $Id: mailpop3_ssl.c,v 1.16 2007/10/27 10:08:26 hoa Exp $
 */

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "mailpop3_ssl.h"

#include "mailpop3.h"

#include "connect.h"
#ifdef HAVE_NETINET_IN_H
#	include <netinet/in.h>
#endif
#ifdef HAVE_UNISTD_H
#	include <unistd.h>
#endif
#include <stdlib.h>

#define DEFAULT_POP3S_PORT 995
#define SERVICE_NAME_POP3S "pop3s"
#define SERVICE_TYPE_TCP "tcp"

int mailpop3_ssl_connect(mailpop3 * f, const char * server, uint16_t port)
{
  return mailpop3_ssl_connect_with_callback(f, server, port,
      NULL, NULL);
}

int mailpop3_ssl_connect_with_callback(mailpop3 * f, const char * server, uint16_t port,
    void (* callback)(struct mailstream_ssl_context * ssl_context, void * data), void * data)
{
  int s;
  mailstream * stream;

  if (port == 0) {
    port = mail_get_service_port(SERVICE_NAME_POP3S, SERVICE_TYPE_TCP);
    if (port == 0)
      port = DEFAULT_POP3S_PORT;
  }

  /* Connection */

  s = mail_tcp_connect(server, port);
  if (s == -1)
    return MAILPOP3_ERROR_CONNECTION_REFUSED;

  stream = mailstream_ssl_open_with_callback(s, callback, data);
  if (stream == NULL) {
    close(s);
    return MAILPOP3_ERROR_SSL;
  }

  return mailpop3_connect(f, stream);
}

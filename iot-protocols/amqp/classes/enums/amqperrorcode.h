/**
 * Mobius Software LTD
 * Copyright 2015-2018, Mobius Software LTD
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

#ifndef AMQPERRORCODE_H
#define AMQPERRORCODE_H

#include <QString>
#include "iot-protocols/amqp/classes/enums/enumobject.h"

enum AMQPErrorCodes
{
    AMQP_INTERNAL_ERROR_CODE                = 0,
    AMQP_NOT_FOUND_ERROR_CODE               = 1,
    AMQP_UNAUTHORIZED_ACCESS_ERROR_CODE     = 2,
    AMQP_DECODE_ERROR_CODE                  = 3,
    AMQP_RESOURCE_LIMIT_EXCEEDED_ERROR_CODE = 4,
    AMQP_NOT_ALLOWED_ERROR_CODE             = 5,
    AMQP_INVALID_FIELD_ERROR_CODE           = 6,
    AMQP_NOT_IMPLEMENTED_ERROR_CODE         = 7,
    AMQP_RESOURCE_LOCKED_ERROR_CODE         = 8,
    AMQP_PRECONDITION_FAILED_ERROR_CODE     = 9,
    AMQP_RESOURCE_DELETED_ERROR_CODE        = 10,
    AMQP_ILLEGAL_STATE_ERROR_CODE           = 11,
    AMQP_FRAME_SIZE_TOO_SMALL_ERROR_CODE    = 12,
    AMQP_CONNECTION_FORCED_ERROR_CODE       = 13,
    AMQP_FRAMING_ERROR_CODE                 = 14,
    AMQP_REDIRECTED_ERROR_CODE              = 15,
    AMQP_WINDOW_VIOLATION_ERROR_CODE        = 16,
    AMQP_ERRANT_LINK_ERROR_CODE             = 17,
    AMQP_HANDLE_IN_USE_ERROR_CODE           = 18,
    AMQP_UNATTACHED_HANDLE_ERROR_CODE       = 19,
    AMQP_DETACH_FORCED_ERROR_CODE           = 20,
    AMQP_TRANSFER_LIMIT_EXCEEDED_ERROR_CODE = 21,
    AMQP_MESSAGE_SIZE_EXCEEDED_ERROR_CODE   = 22,
    AMQP_REDIRECT_ERROR_CODE                = 23,
    AMQP_STOLEN_ERROR_CODE                  = 24,
};

static QString const AMQP_INTERNAL_ERROR_CODE_STRING = "amqp:internal-error";
static QString const AMQP_NOT_FOUND_ERROR_CODE_STRING = "amqp:not-found";
static QString const AMQP_UNAUTHORIZED_ACCESS_ERROR_CODE_STRING = "amqp:unauthorized-access";
static QString const AMQP_DECODE_ERROR_CODE_STRING = "amqp:decode-error";
static QString const AMQP_RESOURCE_LIMIT_EXCEEDED_ERROR_CODE_STRING = "amqp:resource-limit-exceeded";
static QString const AMQP_NOT_ALLOWED_ERROR_CODE_STRING = "amqp:not-allowed";
static QString const AMQP_INVALID_FIELD_ERROR_CODE_STRING = "amqp:invalid-field";
static QString const AMQP_NOT_IMPLEMENTED_ERROR_CODE_STRING = "amqp:not-implemented";
static QString const AMQP_RESOURCE_LOCKED_ERROR_CODE_STRING = "amqp:resource-locked";
static QString const AMQP_PRECONDITION_FAILED_ERROR_CODE_STRING = "amqp:precondition-failed";
static QString const AMQP_RESOURCE_DELETED_ERROR_CODE_STRING = "amqp:resource-deleted";
static QString const AMQP_ILLEGAL_STATE_ERROR_CODE_STRING = "amqp:illegal-state";
static QString const AMQP_FRAME_SIZE_TOO_SMALL_ERROR_CODE_STRING = "amqp:frame-size-too-small";
static QString const AMQP_CONNECTION_FORCED_ERROR_CODE_STRING = "amqp:connection-forced";
static QString const AMQP_FRAMING_ERROR_CODE_STRING = "amqp:framing-error";
static QString const AMQP_REDIRECTED_ERROR_CODE_STRING = "amqp:redirected";
static QString const AMQP_WINDOW_VIOLATION_ERROR_CODE_STRING = "amqp:window-violation";
static QString const AMQP_ERRANT_LINK_ERROR_CODE_STRING = "amqp:errant-link";
static QString const AMQP_HANDLE_IN_USE_ERROR_CODE_STRING = "amqp:handle-in-use";
static QString const AMQP_UNATTACHED_HANDLE_ERROR_CODE_STRING = "amqp:unattached-handle";
static QString const AMQP_DETACH_FORCED_ERROR_CODE_STRING = "amqp:detach-forced";
static QString const AMQP_TRANSFER_LIMIT_EXCEEDED_ERROR_CODE_STRING = "amqp:transfer-limit-exceeded";
static QString const AMQP_MESSAGE_SIZE_EXCEEDED_ERROR_CODE_STRING = "amqp:message-size-exceeded";
static QString const AMQP_REDIRECT_ERROR_CODE_STRING = "amqp:redirect";
static QString const AMQP_STOLEN_ERROR_CODE_STRING = "amqp:stolen";

class AMQPErrorCode : public EnumObject
{
private:

    int value;

public:
    AMQPErrorCode();
    AMQPErrorCode(int value);

    int getValue();
    QString getName();
};

#endif // AMQPERRORCODE_H

package com.brevex.ParkingApp.service.exceptions;

public class AuthenticationServiceException extends RuntimeException
{
    public AuthenticationServiceException(String message)
    {
        super(message);
    }
}

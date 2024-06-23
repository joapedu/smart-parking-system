package com.example.iotparkingsystem.utils

import android.widget.Toast
import android.content.Context

fun Context.toast(message: String)
{
    Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
}

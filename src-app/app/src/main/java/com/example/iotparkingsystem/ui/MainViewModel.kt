package com.example.iotparkingsystem.ui

import androidx.lifecycle.LiveData
import androidx.lifecycle.ViewModel
import com.example.iotparkingsystem.data.FirebaseRepository
import com.example.iotparkingsystem.data.ParkingData

class MainViewModel : ViewModel()
{
    private val repository = FirebaseRepository()
    val parkingData: LiveData<ParkingData> get() = repository.parkingData
}

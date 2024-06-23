package com.example.iotparkingsystem.data

import com.google.firebase.database.*
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData

data class ParkingData(
    val vagasDisponiveis: Int = 0,
    val ultimoID: String = "",
    val alarmAtivacoes: Int = 0,
    val ultimoIDAlarme: String = "",
    val wifiConectado: Boolean = false
)

class FirebaseRepository
{
    private val database: DatabaseReference = FirebaseDatabase.getInstance().reference

    private val _parkingData = MutableLiveData<ParkingData>()
    val parkingData: LiveData<ParkingData> get() = _parkingData

    init
    {
        database.addValueEventListener(object : ValueEventListener
        {
            override fun onDataChange(snapshot: DataSnapshot)
            {
                val vagasDisponiveis = snapshot.child("vagasDisponiveis").getValue(Int::class.java) ?: 0
                val ultimoID = snapshot.child("ultimoID").getValue(String::class.java) ?: ""
                val alarmAtivacoes = snapshot.child("alarmAtivacoes").getValue(Int::class.java) ?: 0
                val ultimoIDAlarme = snapshot.child("ultimoIDAlarme").getValue(String::class.java) ?: ""
                val wifiConectado = snapshot.child("wifiConectado").getValue(Boolean::class.java) ?: false

                _parkingData.value = ParkingData(
                    vagasDisponiveis,
                    ultimoID,
                    alarmAtivacoes,
                    ultimoIDAlarme,
                    wifiConectado
                )
            }

            override fun onCancelled(error: DatabaseError) {}
        })
    }
}

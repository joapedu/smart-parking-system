<template>
  <div class="cards-container">
    <div class="card-group">
      <ParkingInfoCard title="Parking Spaces" :value="availableParkingSpaces" />
      <ParkingInfoCard title="Last Valid ID" :value="lastValidID" />
    </div>

    <div class="card-group">
      <ParkingInfoCard title="Last Invalid ID" :value="lastInvalidID" />
      <ParkingInfoCard title="Invalid Attempts" :value="invalidAttempts" />
    </div>

    <div class="card-group">
      <ParkingInfoCard title="Alarm Activations" :value="alarmActivations" />
      <ParkingInfoCard title="Wi-Fi Status" :value="isWifiConnected" />
    </div>
  </div>
</template>

<script>
import { ref, onMounted } from 'vue';
import ParkingInfoCard from '../components/ParkingInfoCard.vue';
import { getRealtimeData } from '@/services/realtimeService';

export default
{
  components: { ParkingInfoCard },
  setup()
  {
    const availableParkingSpaces = ref('');
    const lastValidID = ref('');
    const lastInvalidID = ref('');
    const invalidAttempts = ref('');
    const alarmActivations = ref('');
    const isWifiConnected = ref('');

    const fetchData = async () => {
      availableParkingSpaces.value = await getRealtimeData('/availableParkingSpaces');
      lastValidID.value = await getRealtimeData('/lastValidID');
      lastInvalidID.value = await getRealtimeData('/lastInvalidID');
      invalidAttempts.value = await getRealtimeData('/invalidAttempts');
      alarmActivations.value = await getRealtimeData('/alarmActivations');
      isWifiConnected.value = await getRealtimeData('/isWifiConnected');
    };

    onMounted(() => { fetchData(); });

    return {
      availableParkingSpaces,
      lastValidID,
      lastInvalidID,
      invalidAttempts,
      alarmActivations,
      isWifiConnected
    };
  }
};
</script>

<style scoped>
.cards-container
{
  display: flex;
  flex-wrap: wrap;
  gap: 1rem;
  justify-content: center;
}

.card-group
{
  display: flex;
  flex-direction: column;
  gap: 1rem;
  width: 100%;
}

@media (min-width: 1024px)
{
  .card-group
  {
    flex-direction: row;
    justify-content: center;
    align-items: center;
  }
}
</style>

<template>
  <header>
    <nav class="navbar">
      <router-link :to="logoLink" class="logo">
        <img src="../assets/icons/navbarLogo.png" alt="Smart Parking logo" class="logo-image"/>
      </router-link>
      <button v-if="loggedIn" @click="logout" class="logout-button">Logout</button>
    </nav>
  </header>
</template>

<script>
import { ref, computed, watch } from 'vue';
import { useRouter } from 'vue-router';
import api from '../services/api';

export default
{
  name: 'AppHeader',
  setup()
  {
    const loggedIn = ref(localStorage.getItem('token') !== null);
    const router = useRouter();

    const logoLink = computed(() => {
      return loggedIn.value ? '/parking-info' : '/';
    });

    const logout = async () => {
      try {
        await api.post('/logout', null, {
          headers: {'Authorization': `Bearer ${localStorage.getItem('token')}`}
        });

        localStorage.removeItem('token');
        loggedIn.value = false;
        await router.push('/');
      }
      catch (error)
      {
        console.error('Erro ao fazer logout:', error);
      }
    };

    watch(
        () => localStorage.getItem('token'),
        (newValue) => {
          loggedIn.value = newValue !== null;
        }
    );

    return {loggedIn, logout, logoLink};
  }
};
</script>

<style scoped>
.navbar
{
  background-color: #17052A;
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0.5em;
}

.logo
{
  margin-left: 20px;
}

.logout-button
{
  padding: 0.5em 1em;
  background-color: #080010;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  margin-right: 20px;
}

.logout-button:hover
{
  background-color: #330066;
}
</style>

<template>
  <div id="app">
    <AppHeader />
    <main class="main-content">
      <router-view/>
    </main>
    <AppFooter />
  </div>
</template>

<script>
import AppHeader from './components/AppHeader.vue'
import AppFooter from './components/AppFooter.vue'

export default
{
  name: 'App',
  components: {
    AppHeader,
    AppFooter
  }
}
</script>

<style>
*
{
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body
{
  background-color: #080010;
}

::-webkit-scrollbar
{
  width: 0.2rem;
}

::-webkit-scrollbar-thumb
{
  background-color: #7AC0FD;
  border-radius: 0.6rem;
}

::-webkit-scrollbar-track
{
  background: #000;
  border-radius: 0.6rem;
}

::-webkit-scrollbar-thumb:hover
{
  background: #45A1F1;
}

#app
{
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  display: flex;
  flex-direction: column;
  font-family: Avenir, Helvetica, Arial, sans-serif;
  min-height: 100vh;
}

.main-content
{
  flex: 1;
  display: flex;
  justify-content: center;
  align-items: center;
}
</style>

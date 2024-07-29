import { createRouter, createWebHistory } from 'vue-router';
import SignIn from '../views/HomePage.vue';
import SignUp from '../views/SignUp.vue';
import ParkingInfo from '../views/ParkingInfo.vue';

const routes = [
    { path: '/', name: 'SignIn', component: SignIn },
    { path: '/sign-up', name: 'SignUp', component: SignUp },
    { path: '/parking-info', name: 'ParkingInfo', component: ParkingInfo }
];

const router = createRouter({
    history: createWebHistory(process.env.BASE_URL),
    routes
});

router.beforeEach((to, from, next) => {
    const publicPages = ['/', '/sign-up'];
    const authRequired = !publicPages.includes(to.path);
    const loggedIn = localStorage.getItem('token');

    if (authRequired && !loggedIn) { return next('/'); }

    next();
});

export default router;
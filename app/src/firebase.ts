// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries
import { getDatabase } from "firebase/database";

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyCcfOf9iHIEXE34s1fbmusIBgdVJsgmkss",
  authDomain: "dai-pds-cb4fb.firebaseapp.com",
  databaseURL:
    "https://dai-pds-cb4fb-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "dai-pds-cb4fb",
  storageBucket: "dai-pds-cb4fb.firebasestorage.app",
  messagingSenderId: "419928720292",
  appId: "1:419928720292:web:ac9b792147976855a8f731",
};

// Initialize Firebase
export const app = initializeApp(firebaseConfig);
export const db = getDatabase();

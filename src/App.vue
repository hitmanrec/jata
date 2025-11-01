<template>
	<div class="app">
		<add-item-dialog v-model:show="showAddItemDialog">
			<add-item-form 
				@add-item="addItem"
			/>
		</add-item-dialog>
		<small-button class="add-item-button" @click="showAddItemDialog = true">
			Add film
		</small-button>
		<item-list 
			:items="items" 
			@delete-item="deleteItem"
		/>
		
	</div>
</template>


<script>
import AddItemForm from '@/components/AddItemForm'
import ItemList from '@/components/ItemList'

export default {
	components: {
		AddItemForm,
		ItemList
	},
	data() {
		return {
			items: [{
				title: 'Просто запредельно',
				status: 'incomplete',
				tags: ['фантастика', 'юмор'],
				rating: '',
				expectation: '',
				description: 'сериал?',
				creationTime: 123,
				completionTime: ''
			}, {
				title: 'Dark matter',
				status: 'incomplete',
				tags: [],
				rating: '',
				expectation: '',
				description: '',
				creationTime: 124,
				completionTime: ''
			}, {
				title: 'баллада бастера скраггса',
				status: 'incomplete',
				tags: [],
				rating: '',
				expectation: '',
				description: '',
				creationTime: 125,
				completionTime: ''
			}],
			showAddItemDialog: false
		}
	},
	methods: {
		addItem(item) {
			this.items.push(item)
			this.showAddItemDialog = false
		},
		deleteItem(item) {
			this.items = this.items.filter(i => i.creationTime !== item.creationTime)
		}
	}
}
</script>

<style>
:root {
	--big-border-rad: 8px;
	--small-border-rad: 5px;
	--dark-bg-col: #141010;
	--accent-col: #d4ea9a;
	--main-text-col: #f4f4df;
	--secondary-text-col: #888888;
	--strong-font: 'Montserrat', 'Segoe UI', sans-serif;
	--main-font: 'Poiret One', 'Segoe UI', sans-serif;
	--main-font-weight: 500;
}


* {
	margin: 0;
	padding: 0;
	box-sizing: border-box;
}

body {
	color: var(--main-text-col);
	background-color: var(--dark-bg-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);

}

.app{
	display: flex;
	flex-direction: column;
	padding: 20px;
}

.base-input{
	padding: 10px;
	border: 1px solid var(--accent-col);
	border-radius: var(--small-border-rad);
	background-color: var(--dark-bg-col);
	color: var(--main-text-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);
}

.base-button{
	padding: 10px 15px;
	border: 1px solid var(--accent-col);
	border-radius: var(--small-border-rad);
	background: none;
	color: var(--main-text-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);
}

.base-dialog{
	position: fixed;
	top: 0;
	left: 0;
	bottom: 0;
	right: 0;
	background: rgba(255, 255, 255, 0.2);
	display: flex;
}

.add-item-button{
	align-self: flex-end;
	margin-bottom: 20px;
}

</style>
<template>
	<div class="side-panel">
		<add-category-dialog v-model:show="AddCategoryDialogVisible">
			<add-category-form @add-category="addCategory" />
		</add-category-dialog>
		<div class="category-list">
			<div class="category-list-header">
				<h4>Categories</h4>
				<category-button class="category-add" @click="showAddCategoryDialog">
					+ Add
				</category-button>
			</div>
			<category-button 
				v-for="category in categories"
				v-show="!category.removed" 
				class="category-item" 
				:class="{'selected-category': category.id === selectedCategoryId}"
				:category="category"
				:key="category.id"
				@click="selectCategory(category.id)"
			>
				<span class="category-name">{{ category.name }}</span>
				<span v-show="category.items > 0" class="category-items">{{ category.items }}</span>
			</category-button>
		</div>
	</div>
</template>

<script>
import AddCategoryForm from '@/components/AddCategoryForm'
export default {
	components: {
		AddCategoryForm
	},
	name: 'side-panel',
	data() {
		return {
			AddCategoryDialogVisible: false
		}
	},
	props: {
		categories:{
			type: Array,
			required: true
		},
		selectedCategoryId: {
			type: Number
		}
	},
	methods: {
		selectCategory(categoryId) {
			this.$emit('select-category', categoryId)
		},
		showAddCategoryDialog() {
			this.AddCategoryDialogVisible = true
		},
		addCategory(category) {
			category.items = []
			this.$emit('add-category', category)
			this.AddCategoryDialogVisible = false
		}
	}
};
</script>

<style scoped>
.side-panel{
	width: 300px;
	height: 100%;
	background-color: var(--dark-bg-col);
	border-right: 1px solid var(--accent-col);
	padding: 10px;
}

.category-list{
	display: flex;
	flex-direction: column;
	gap: 5px;
}

.category-button{
	display: flex;
	align-self: flex-end;
	width: 90%;
	justify-content: space-between;
	align-items: center;
	padding: 10px;
	border: 1px solid var(--accent-col);
	border-radius: var(--small-border-rad);
	background-color: var(--dark-bg-col);
	color: var(--main-text-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);
	transition: color 0.2s ease, background-color 0.2s ease, border-color 0.2s ease, width 0.2s ease;
}

.category-button:hover{
	color: var(--accent-col-hover);
	border-color: var(--accent-col-hover);
	background-color: rgba(255, 255, 255, 0.1);
	width: 100%;
	cursor: pointer;
}

.category-add {
	display: flex;
	width: auto !important;
	align-self: flex-end;
	text-align: center;
	display: block;
	padding-bottom: 10px;

}

.selected-category{
	width: 100%;
}

.category-list-header{
	display: flex;
	flex-direction: row;
	justify-content: space-between;
	align-items: center;
	margin-bottom: 10px;
}
</style>
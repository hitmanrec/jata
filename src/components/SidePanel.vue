<template>
	<div class="side-panel">
		<update-category-dialog v-model:show="UpdateCategoryDialogVisible">
			<update-category-form @add-category="addCategory" @update-category="updateCategory" :actionType="actionType" :category="category" />
		</update-category-dialog>
		<div class="category-list">
			<div class="category-list-header">
				<h4>Categories</h4>
				<div class="category-list-header-buttons">
					<category-button class="category-add" @click="showAddCategoryDialog">
						<i class="fa-solid fa-plus" alt="add"></i>
					</category-button>
					<category-button class="category-edit" @click="showUpdateCategoryDialog">
						<i class="fa-solid fa-pencil" alt="edit"></i>
					</category-button>
					<category-button class="category-delete" @click="deleteCategory">
						<i class="fa-solid fa-trash" alt="delete"></i>
					</category-button>
				</div>
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
				<span class="category-items">{{ category.items }}</span>
			</category-button>
		</div>
	</div>
</template>

<script>
import UpdateCategoryForm from '@/components/UpdateCategoryForm'
export default {
	components: {
		UpdateCategoryForm
	},
	name: 'side-panel',
	data() {
		return {
			UpdateCategoryDialogVisible: false,
			actionType: 'add',
			category: {}
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
			this.actionType = 'add'
			this.category = {name: '', items: [], removed: false}
			this.UpdateCategoryDialogVisible = true
		},
		showUpdateCategoryDialog() {
			this.actionType = 'update'
			this.category = this.categories.find(c => c.id === this.selectedCategoryId)
			this.UpdateCategoryDialogVisible = true
		},
		addCategory(category) {
			category.items = []
			this.$emit('add-category', category)
			this.UpdateCategoryDialogVisible = false
		},
		updateCategory(category) {
			this.$emit('update-category', category)
			this.UpdateCategoryDialogVisible = false
		},
		deleteCategory() {
			const selectedCategory = this.categories.find(c => c.id === this.selectedCategoryId)
			if (!selectedCategory) {
				return
			}
			
			const confirmed = window.confirm(`Are you sure you want to delete the category "${selectedCategory.name}"?`)
			if (confirmed) {
				this.$emit('delete-category', selectedCategory)
			}
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
	height: 100%;
	text-align: center;
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

.selected-category{
	width: 100%;
}

.category-list-header{
	display: flex;
	flex-direction: row;
	align-items: center;
	justify-content: space-between;
	margin-bottom: 10px;
	padding-bottom: 10px;
}

.category-list-header-buttons{
	display: flex;
	flex-direction: row;
	align-items: center;
	justify-content: flex-end;
	gap: 10px;
}
</style>